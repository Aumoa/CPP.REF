// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkBuffer.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"

namespace Ayla
{
	VkBuffer::VkBuffer(VkGraphics* graphics, BufferUsage usage)
		: m_Usage(usage), m_Graphics(graphics)
	{
	}

	VkBuffer::~VkBuffer() noexcept
	{
		if (m_Buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_Graphics->GetDevice(), m_Buffer, nullptr);
		}
		if (m_Memory != VK_NULL_HANDLE)
		{
			vkFreeMemory(m_Graphics->GetDevice(), m_Memory, nullptr);
		}
	}

	void VkBuffer::UpdateData(std::span<const byte> buffer)
	{
		if (buffer.size() == 0)
		{
			return;
		}

		auto device = m_Graphics->GetDevice();

		// Ensure device-local target buffer exists and is large enough
		if (m_Buffer == VK_NULL_HANDLE || m_Size < buffer.size())
		{
			if (m_Buffer != VK_NULL_HANDLE)
			{
				vkDestroyBuffer(device, m_Buffer, nullptr);
				m_Buffer = VK_NULL_HANDLE;
			}
			if (m_Memory != VK_NULL_HANDLE)
			{
				vkFreeMemory(device, m_Memory, nullptr);
				m_Memory = VK_NULL_HANDLE;
			}

			m_Size = buffer.size();

			VkBufferCreateInfo bufferInfo{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = m_Size,
				.usage = (VkBufferUsageFlags)(((m_Usage == BufferUsage::VertexBuffer) ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT : VK_BUFFER_USAGE_INDEX_BUFFER_BIT) | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT),
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};

			VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer));

			VkMemoryRequirements memReq;
			vkGetBufferMemoryRequirements(device, m_Buffer, &memReq);

			VkPhysicalDeviceMemoryProperties memProps;
			vkGetPhysicalDeviceMemoryProperties(m_Graphics->GetPhysicalDevice(), &memProps);

			uint32_t memoryTypeIndex = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((memReq.memoryTypeBits & (1u << i)) == 0)
				{
					continue;
				}

				if (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
				{
					memoryTypeIndex = i;
					break;
				}
			}
			if (memoryTypeIndex == UINT32_MAX)
			{
				for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
				{
					if (memReq.memoryTypeBits & (1u << i))
					{
						memoryTypeIndex = i;
						break;
					}
				}
			}

			VkMemoryAllocateInfo allocInfo
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize = memReq.size,
				.memoryTypeIndex = memoryTypeIndex
			};

			// If buffer requires shader device address, memory must be allocated with DEVICE_ADDRESS bit.
			VkMemoryAllocateFlagsInfo allocFlagsInfo{};
			if (bufferInfo.usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
			{
				allocFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
				allocFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
				allocInfo.pNext = &allocFlagsInfo;
			}

			VKR(vkAllocateMemory(device, &allocInfo, nullptr, &m_Memory));
			VKR(vkBindBufferMemory(device, m_Buffer, m_Memory, 0));
		}

		// Create staging buffer
		::VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory stagingMemory = VK_NULL_HANDLE;
		{
			VkBufferCreateInfo stagingInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = buffer.size(),
				.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &stagingInfo, nullptr, &stagingBuffer));

			VkMemoryRequirements stagingReq;
			vkGetBufferMemoryRequirements(device, stagingBuffer, &stagingReq);

			VkPhysicalDeviceMemoryProperties memProps;
			vkGetPhysicalDeviceMemoryProperties(m_Graphics->GetPhysicalDevice(), &memProps);

			uint32_t stagingType = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((stagingReq.memoryTypeBits & (1u << i)) == 0)
				{
					continue;
				}

				if ((memProps.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) == (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
				{
					stagingType = i;
					break;
				}
			}

			if (stagingType == UINT32_MAX)
			{
				for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
				{
					if (stagingReq.memoryTypeBits & (1u << i))
					{
						stagingType = i;
						break;
					}
				}
			}

			VkMemoryAllocateInfo stagingAlloc
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize = stagingReq.size,
				.memoryTypeIndex = stagingType
			};
			VkMemoryAllocateFlagsInfo stagingAllocFlags{};
			if (stagingInfo.usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
			{
				stagingAllocFlags.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
				stagingAllocFlags.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
				stagingAlloc.pNext = &stagingAllocFlags;
			}

			VKR(vkAllocateMemory(device, &stagingAlloc, nullptr, &stagingMemory));
			VKR(vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0));
		}

		// Upload to staging
		void* mapped = nullptr;
		VKR(vkMapMemory(device, stagingMemory, 0, buffer.size(), 0, &mapped));
		std::memcpy(mapped, buffer.data(), buffer.size());
		vkUnmapMemory(device, stagingMemory);

		// Copy from staging to device buffer using pooled command buffer
		{
			auto cmd = m_Graphics->GetPooledCommandBuffer();
			cmd->BeginCommands();
			auto* vkCmd = static_cast<VkCommandBuffer*>(cmd.Get());

			VkBufferCopy copyRegion{
				.srcOffset = 0,
				.dstOffset = 0,
				.size = buffer.size()
			};
			vkCmdCopyBuffer(vkCmd->GetVkCommandBuffer(), stagingBuffer, m_Buffer, 1, &copyRegion);

			cmd->EndCommands();

			m_Graphics->AddFenceCompletionCallback(vkCmd->GetFence(), [cmd, graphics = m_Graphics, device, stagingBuffer, stagingMemory]
			{
				vkDestroyBuffer(device, stagingBuffer, nullptr);
				vkFreeMemory(device, stagingMemory, nullptr);
				graphics->ReleasePooledCommandBuffer(cmd);
			});
		}
	}

	VkDeviceAddress VkBuffer::GetDeviceAddress() const noexcept
	{
		if (m_Buffer == VK_NULL_HANDLE)
		{
			return 0;
		}

		VkBufferDeviceAddressInfo addrInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = m_Buffer
		};

		return vkGetBufferDeviceAddress(m_Graphics->GetDevice(), &addrInfo);
	}

	SharedPtr<Buffer> VkGraphics::CreateBuffer_Implementation(BufferUsage usage)
	{
		return New<VkBuffer>(this, usage);
	}
}