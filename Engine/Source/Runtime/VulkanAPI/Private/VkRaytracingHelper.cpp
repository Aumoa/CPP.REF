// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkRaytracingHelper.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"

namespace Ayla
{
	// ==================== VkAccelerationStructure ====================

	VkAccelerationStructure::VkAccelerationStructure(VkGraphics* graphics)
		: m_Graphics(graphics)
	{
	}

	VkAccelerationStructure::~VkAccelerationStructure() noexcept
	{
		auto device = m_Graphics->GetDevice();
		
		if (m_AccelerationStructure != VK_NULL_HANDLE)
		{
			auto vkDestroyAccelerationStructureKHR = (PFN_vkDestroyAccelerationStructureKHR)
				vkGetDeviceProcAddr(device, "vkDestroyAccelerationStructureKHR");
			vkDestroyAccelerationStructureKHR(device, m_AccelerationStructure, nullptr);
		}

		if (m_Buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device, m_Buffer, nullptr);
		}

		if (m_Memory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device, m_Memory, nullptr);
		}
	}

	void VkAccelerationStructure::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage)
	{
		auto device = m_Graphics->GetDevice();

		VkBufferCreateInfo bufferInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = size,
			.usage = usage,
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
			if ((memReq.memoryTypeBits & (1u << i)) &&
				(memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
			{
				memoryTypeIndex = i;
				break;
			}
		}

		VkMemoryAllocateFlagsInfo allocFlagsInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
			.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
		};

		VkMemoryAllocateInfo allocInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = &allocFlagsInfo,
			.allocationSize = memReq.size,
			.memoryTypeIndex = memoryTypeIndex
		};

		VKR(vkAllocateMemory(device, &allocInfo, nullptr, &m_Memory));
		VKR(vkBindBufferMemory(device, m_Buffer, m_Memory, 0));
	}

	void VkAccelerationStructure::CreateBottomLevel(const std::vector<Vector3>& vertices, const std::vector<uint32_t>& indices)
	{
		auto device = m_Graphics->GetDevice();
		
		// Get function pointers
		auto vkGetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR)
			vkGetDeviceProcAddr(device, "vkGetAccelerationStructureBuildSizesKHR");
		auto vkCreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR)
			vkGetDeviceProcAddr(device, "vkCreateAccelerationStructureKHR");
		auto vkGetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR)
			vkGetDeviceProcAddr(device, "vkGetAccelerationStructureDeviceAddressKHR");
		auto vkCmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR)
			vkGetDeviceProcAddr(device, "vkCmdBuildAccelerationStructuresKHR");

		// Create vertex and index buffers
		size_t vertexBufferSize = vertices.size() * sizeof(Vector3);
		size_t indexBufferSize = indices.size() * sizeof(uint32_t);

		::VkBuffer vertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory vertexMemory = VK_NULL_HANDLE;
		::VkBuffer indexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory indexMemory = VK_NULL_HANDLE;

		// Create and upload vertex buffer
		{
			VkBufferCreateInfo bufferInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = vertexBufferSize,
				.usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
						 VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer));

			VkMemoryRequirements memReq;
			vkGetBufferMemoryRequirements(device, vertexBuffer, &memReq);

			VkPhysicalDeviceMemoryProperties memProps;
			vkGetPhysicalDeviceMemoryProperties(m_Graphics->GetPhysicalDevice(), &memProps);

			uint32_t memoryTypeIndex = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((memReq.memoryTypeBits & (1u << i)) &&
					(memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
				{
					memoryTypeIndex = i;
					break;
				}
			}

			VkMemoryAllocateFlagsInfo allocFlags
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
				.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
			};

			VkMemoryAllocateInfo allocInfo
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = &allocFlags,
				.allocationSize = memReq.size,
				.memoryTypeIndex = memoryTypeIndex
			};
			VKR(vkAllocateMemory(device, &allocInfo, nullptr, &vertexMemory));
			VKR(vkBindBufferMemory(device, vertexBuffer, vertexMemory, 0));

			// TODO: Optimize by combining vertex and index uploads into single staging buffer and command submission
			// Upload data using staging buffer (simplified for now)
			::VkBuffer stagingBuffer;
			VkDeviceMemory stagingMemory;

			VkBufferCreateInfo stagingInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = vertexBufferSize,
				.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &stagingInfo, nullptr, &stagingBuffer));

			VkMemoryRequirements stagingReq;
			vkGetBufferMemoryRequirements(device, stagingBuffer, &stagingReq);

			uint32_t stagingType = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((stagingReq.memoryTypeBits & (1u << i)) &&
					(memProps.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)))
				{
					stagingType = i;
					break;
				}
			}

			VkMemoryAllocateInfo stagingAlloc
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize = stagingReq.size,
				.memoryTypeIndex = stagingType
			};
			VKR(vkAllocateMemory(device, &stagingAlloc, nullptr, &stagingMemory));
			VKR(vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0));

			void* mapped;
			VKR(vkMapMemory(device, stagingMemory, 0, vertexBufferSize, 0, &mapped));
			std::memcpy(mapped, vertices.data(), vertexBufferSize);
			vkUnmapMemory(device, stagingMemory);

			// Copy to device buffer
			auto cmd = m_Graphics->GetPooledCommandBuffer();
			cmd->BeginCommands();
			auto* vkCmd = static_cast<VkCommandBuffer*>(cmd.Get());

			VkBufferCopy copyRegion{ .srcOffset = 0, .dstOffset = 0, .size = vertexBufferSize };
			vkCmdCopyBuffer(vkCmd->GetVkCommandBuffer(), stagingBuffer, vertexBuffer, 1, &copyRegion);

			cmd->EndCommands();
			cmd->WaitForCompletion(TimeSpan::FromSeconds(10));

			vkDestroyBuffer(device, stagingBuffer, nullptr);
			vkFreeMemory(device, stagingMemory, nullptr);
			m_Graphics->ReleasePooledCommandBuffer(cmd);
		}

		// Create and upload index buffer (similar to vertex buffer)
		{
			VkBufferCreateInfo bufferInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = indexBufferSize,
				.usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
						 VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &indexBuffer));

			VkMemoryRequirements memReq;
			vkGetBufferMemoryRequirements(device, indexBuffer, &memReq);

			VkPhysicalDeviceMemoryProperties memProps;
			vkGetPhysicalDeviceMemoryProperties(m_Graphics->GetPhysicalDevice(), &memProps);

			uint32_t memoryTypeIndex = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((memReq.memoryTypeBits & (1u << i)) &&
					(memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
				{
					memoryTypeIndex = i;
					break;
				}
			}

			VkMemoryAllocateFlagsInfo allocFlags
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
				.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
			};

			VkMemoryAllocateInfo allocInfo
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = &allocFlags,
				.allocationSize = memReq.size,
				.memoryTypeIndex = memoryTypeIndex
			};
			VKR(vkAllocateMemory(device, &allocInfo, nullptr, &indexMemory));
			VKR(vkBindBufferMemory(device, indexBuffer, indexMemory, 0));

			::VkBuffer stagingBuffer;
			VkDeviceMemory stagingMemory;

			VkBufferCreateInfo stagingInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = indexBufferSize,
				.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &stagingInfo, nullptr, &stagingBuffer));

			VkMemoryRequirements stagingReq;
			vkGetBufferMemoryRequirements(device, stagingBuffer, &stagingReq);

			uint32_t stagingType = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((stagingReq.memoryTypeBits & (1u << i)) &&
					(memProps.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)))
				{
					stagingType = i;
					break;
				}
			}

			VkMemoryAllocateInfo stagingAlloc
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize = stagingReq.size,
				.memoryTypeIndex = stagingType
			};
			VKR(vkAllocateMemory(device, &stagingAlloc, nullptr, &stagingMemory));
			VKR(vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0));

			void* mapped;
			VKR(vkMapMemory(device, stagingMemory, 0, indexBufferSize, 0, &mapped));
			std::memcpy(mapped, indices.data(), indexBufferSize);
			vkUnmapMemory(device, stagingMemory);

			auto cmd = m_Graphics->GetPooledCommandBuffer();
			cmd->BeginCommands();
			auto* vkCmd = static_cast<VkCommandBuffer*>(cmd.Get());

			VkBufferCopy copyRegion{ .srcOffset = 0, .dstOffset = 0, .size = indexBufferSize };
			vkCmdCopyBuffer(vkCmd->GetVkCommandBuffer(), stagingBuffer, indexBuffer, 1, &copyRegion);

			cmd->EndCommands();
			cmd->WaitForCompletion(TimeSpan::FromSeconds(10));

			vkDestroyBuffer(device, stagingBuffer, nullptr);
			vkFreeMemory(device, stagingMemory, nullptr);
			m_Graphics->ReleasePooledCommandBuffer(cmd);
		}

		// Get buffer device addresses
		VkBufferDeviceAddressInfo vertexAddrInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = vertexBuffer
		};
		VkDeviceAddress vertexAddress = vkGetBufferDeviceAddress(device, &vertexAddrInfo);

		VkBufferDeviceAddressInfo indexAddrInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = indexBuffer
		};
		VkDeviceAddress indexAddress = vkGetBufferDeviceAddress(device, &indexAddrInfo);

		// Build BLAS
		VkAccelerationStructureGeometryKHR geometry
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
			.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR,
			.geometry = {
				.triangles = {
					.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR,
					.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT,
					.vertexData = { .deviceAddress = vertexAddress },
					.vertexStride = sizeof(Vector3),
					.maxVertex = (uint32_t)vertices.size() - 1,
					.indexType = VK_INDEX_TYPE_UINT32,
					.indexData = { .deviceAddress = indexAddress }
				}
			},
			.flags = VK_GEOMETRY_OPAQUE_BIT_KHR
		};

		VkAccelerationStructureBuildGeometryInfoKHR buildInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
			.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
			.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
			.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
			.geometryCount = 1,
			.pGeometries = &geometry
		};

		uint32_t primitiveCount = (uint32_t)indices.size() / 3;

		VkAccelerationStructureBuildSizesInfoKHR sizeInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR
		};

		vkGetAccelerationStructureBuildSizesKHR(device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
			&buildInfo, &primitiveCount, &sizeInfo);

		// Create acceleration structure buffer
		CreateBuffer(sizeInfo.accelerationStructureSize,
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT);

		VkAccelerationStructureCreateInfoKHR createInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR,
			.buffer = m_Buffer,
			.size = sizeInfo.accelerationStructureSize,
			.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR
		};

		VKR(vkCreateAccelerationStructureKHR(device, &createInfo, nullptr, &m_AccelerationStructure));

		// Create scratch buffer
		::VkBuffer scratchBuffer;
		VkDeviceMemory scratchMemory;
		{
			VkBufferCreateInfo bufferInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = sizeInfo.buildScratchSize,
				.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &scratchBuffer));

			VkMemoryRequirements memReq;
			vkGetBufferMemoryRequirements(device, scratchBuffer, &memReq);

			VkPhysicalDeviceMemoryProperties memProps;
			vkGetPhysicalDeviceMemoryProperties(m_Graphics->GetPhysicalDevice(), &memProps);

			uint32_t memoryTypeIndex = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((memReq.memoryTypeBits & (1u << i)) &&
					(memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
				{
					memoryTypeIndex = i;
					break;
				}
			}

			VkMemoryAllocateFlagsInfo allocFlags
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
				.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
			};

			VkMemoryAllocateInfo allocInfo
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = &allocFlags,
				.allocationSize = memReq.size,
				.memoryTypeIndex = memoryTypeIndex
			};
			VKR(vkAllocateMemory(device, &allocInfo, nullptr, &scratchMemory));
			VKR(vkBindBufferMemory(device, scratchBuffer, scratchMemory, 0));
		}

		VkBufferDeviceAddressInfo scratchAddrInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = scratchBuffer
		};
		VkDeviceAddress scratchAddress = vkGetBufferDeviceAddress(device, &scratchAddrInfo);

		buildInfo.dstAccelerationStructure = m_AccelerationStructure;
		buildInfo.scratchData.deviceAddress = scratchAddress;

		VkAccelerationStructureBuildRangeInfoKHR buildRangeInfo
		{
			.primitiveCount = primitiveCount,
			.primitiveOffset = 0,
			.firstVertex = 0,
			.transformOffset = 0
		};

		const VkAccelerationStructureBuildRangeInfoKHR* pBuildRangeInfo = &buildRangeInfo;

		// Build acceleration structure
		{
			auto cmd = m_Graphics->GetPooledCommandBuffer();
			cmd->BeginCommands();
			auto* vkCmd = static_cast<VkCommandBuffer*>(cmd.Get());

			vkCmdBuildAccelerationStructuresKHR(vkCmd->GetVkCommandBuffer(), 1, &buildInfo, &pBuildRangeInfo);

			cmd->EndCommands();
			cmd->WaitForCompletion(TimeSpan::FromSeconds(10));
			m_Graphics->ReleasePooledCommandBuffer(cmd);
		}

		// Get acceleration structure device address
		VkAccelerationStructureDeviceAddressInfoKHR asAddrInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR,
			.accelerationStructure = m_AccelerationStructure
		};
		m_DeviceAddress = vkGetAccelerationStructureDeviceAddressKHR(device, &asAddrInfo);

		// Cleanup temporary buffers
		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexMemory, nullptr);
		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkFreeMemory(device, indexMemory, nullptr);
		vkDestroyBuffer(device, scratchBuffer, nullptr);
		vkFreeMemory(device, scratchMemory, nullptr);
	}

	void VkAccelerationStructure::CreateTopLevel(const VkAccelerationStructure& blas)
	{
		auto device = m_Graphics->GetDevice();

		auto vkGetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR)
			vkGetDeviceProcAddr(device, "vkGetAccelerationStructureBuildSizesKHR");
		auto vkCreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR)
			vkGetDeviceProcAddr(device, "vkCreateAccelerationStructureKHR");
		auto vkGetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR)
			vkGetDeviceProcAddr(device, "vkGetAccelerationStructureDeviceAddressKHR");
		auto vkCmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR)
			vkGetDeviceProcAddr(device, "vkCmdBuildAccelerationStructuresKHR");

		// Create instance buffer
		VkTransformMatrixKHR transformMatrix
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f
		};

		VkAccelerationStructureInstanceKHR instance
		{
			.transform = transformMatrix,
			.instanceCustomIndex = 0,
			.mask = 0xFF,
			.instanceShaderBindingTableRecordOffset = 0,
			.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR,
			.accelerationStructureReference = blas.GetDeviceAddress()
		};

		::VkBuffer instanceBuffer;
		VkDeviceMemory instanceMemory;

		{
			VkBufferCreateInfo bufferInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = sizeof(VkAccelerationStructureInstanceKHR),
				.usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
						 VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &instanceBuffer));

			VkMemoryRequirements memReq;
			vkGetBufferMemoryRequirements(device, instanceBuffer, &memReq);

			VkPhysicalDeviceMemoryProperties memProps;
			vkGetPhysicalDeviceMemoryProperties(m_Graphics->GetPhysicalDevice(), &memProps);

			uint32_t memoryTypeIndex = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((memReq.memoryTypeBits & (1u << i)) &&
					(memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
				{
					memoryTypeIndex = i;
					break;
				}
			}

			VkMemoryAllocateFlagsInfo allocFlags
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
				.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
			};

			VkMemoryAllocateInfo allocInfo
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = &allocFlags,
				.allocationSize = memReq.size,
				.memoryTypeIndex = memoryTypeIndex
			};
			VKR(vkAllocateMemory(device, &allocInfo, nullptr, &instanceMemory));
			VKR(vkBindBufferMemory(device, instanceBuffer, instanceMemory, 0));

			// Upload instance data
			::VkBuffer stagingBuffer;
			VkDeviceMemory stagingMemory;

			VkBufferCreateInfo stagingInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = sizeof(instance),
				.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &stagingInfo, nullptr, &stagingBuffer));

			VkMemoryRequirements stagingReq;
			vkGetBufferMemoryRequirements(device, stagingBuffer, &stagingReq);

			uint32_t stagingType = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((stagingReq.memoryTypeBits & (1u << i)) &&
					(memProps.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)))
				{
					stagingType = i;
					break;
				}
			}

			VkMemoryAllocateInfo stagingAlloc
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize = stagingReq.size,
				.memoryTypeIndex = stagingType
			};
			VKR(vkAllocateMemory(device, &stagingAlloc, nullptr, &stagingMemory));
			VKR(vkBindBufferMemory(device, stagingBuffer, stagingMemory, 0));

			void* mapped;
			VKR(vkMapMemory(device, stagingMemory, 0, sizeof(instance), 0, &mapped));
			std::memcpy(mapped, &instance, sizeof(instance));
			vkUnmapMemory(device, stagingMemory);

			auto cmd = m_Graphics->GetPooledCommandBuffer();
			cmd->BeginCommands();
			auto* vkCmd = static_cast<VkCommandBuffer*>(cmd.Get());

			VkBufferCopy copyRegion{ .srcOffset = 0, .dstOffset = 0, .size = sizeof(instance) };
			vkCmdCopyBuffer(vkCmd->GetVkCommandBuffer(), stagingBuffer, instanceBuffer, 1, &copyRegion);

			cmd->EndCommands();
			cmd->WaitForCompletion(TimeSpan::FromSeconds(10));

			vkDestroyBuffer(device, stagingBuffer, nullptr);
			vkFreeMemory(device, stagingMemory, nullptr);
			m_Graphics->ReleasePooledCommandBuffer(cmd);
		}

		VkBufferDeviceAddressInfo instanceAddrInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = instanceBuffer
		};
		VkDeviceAddress instanceAddress = vkGetBufferDeviceAddress(device, &instanceAddrInfo);

		// Build TLAS
		VkAccelerationStructureGeometryKHR geometry
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
			.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR,
			.geometry = {
				.instances = {
					.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR,
					.arrayOfPointers = VK_FALSE,
					.data = { .deviceAddress = instanceAddress }
				}
			},
			.flags = VK_GEOMETRY_OPAQUE_BIT_KHR
		};

		VkAccelerationStructureBuildGeometryInfoKHR buildInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
			.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR,
			.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
			.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
			.geometryCount = 1,
			.pGeometries = &geometry
		};

		uint32_t primitiveCount = 1;

		VkAccelerationStructureBuildSizesInfoKHR sizeInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR
		};

		vkGetAccelerationStructureBuildSizesKHR(device, VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
			&buildInfo, &primitiveCount, &sizeInfo);

		CreateBuffer(sizeInfo.accelerationStructureSize,
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT);

		VkAccelerationStructureCreateInfoKHR createInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR,
			.buffer = m_Buffer,
			.size = sizeInfo.accelerationStructureSize,
			.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR
		};

		VKR(vkCreateAccelerationStructureKHR(device, &createInfo, nullptr, &m_AccelerationStructure));

		// Create scratch buffer
		::VkBuffer scratchBuffer;
		VkDeviceMemory scratchMemory;
		{
			VkBufferCreateInfo bufferInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = sizeInfo.buildScratchSize,
				.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};
			VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &scratchBuffer));

			VkMemoryRequirements memReq;
			vkGetBufferMemoryRequirements(device, scratchBuffer, &memReq);

			VkPhysicalDeviceMemoryProperties memProps;
			vkGetPhysicalDeviceMemoryProperties(m_Graphics->GetPhysicalDevice(), &memProps);

			uint32_t memoryTypeIndex = UINT32_MAX;
			for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
			{
				if ((memReq.memoryTypeBits & (1u << i)) &&
					(memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
				{
					memoryTypeIndex = i;
					break;
				}
			}

			VkMemoryAllocateFlagsInfo allocFlags
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
				.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
			};

			VkMemoryAllocateInfo allocInfo
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = &allocFlags,
				.allocationSize = memReq.size,
				.memoryTypeIndex = memoryTypeIndex
			};
			VKR(vkAllocateMemory(device, &allocInfo, nullptr, &scratchMemory));
			VKR(vkBindBufferMemory(device, scratchBuffer, scratchMemory, 0));
		}

		VkBufferDeviceAddressInfo scratchAddrInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = scratchBuffer
		};
		VkDeviceAddress scratchAddress = vkGetBufferDeviceAddress(device, &scratchAddrInfo);

		buildInfo.dstAccelerationStructure = m_AccelerationStructure;
		buildInfo.scratchData.deviceAddress = scratchAddress;

		VkAccelerationStructureBuildRangeInfoKHR buildRangeInfo
		{
			.primitiveCount = 1,
			.primitiveOffset = 0,
			.firstVertex = 0,
			.transformOffset = 0
		};

		const VkAccelerationStructureBuildRangeInfoKHR* pBuildRangeInfo = &buildRangeInfo;

		// Build acceleration structure
		{
			auto cmd = m_Graphics->GetPooledCommandBuffer();
			cmd->BeginCommands();
			auto* vkCmd = static_cast<VkCommandBuffer*>(cmd.Get());

			vkCmdBuildAccelerationStructuresKHR(vkCmd->GetVkCommandBuffer(), 1, &buildInfo, &pBuildRangeInfo);

			cmd->EndCommands();
			cmd->WaitForCompletion(TimeSpan::FromSeconds(10));
			m_Graphics->ReleasePooledCommandBuffer(cmd);
		}

		// Get acceleration structure device address
		VkAccelerationStructureDeviceAddressInfoKHR asAddrInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR,
			.accelerationStructure = m_AccelerationStructure
		};
		m_DeviceAddress = vkGetAccelerationStructureDeviceAddressKHR(device, &asAddrInfo);

		// Cleanup
		vkDestroyBuffer(device, instanceBuffer, nullptr);
		vkFreeMemory(device, instanceMemory, nullptr);
		vkDestroyBuffer(device, scratchBuffer, nullptr);
		vkFreeMemory(device, scratchMemory, nullptr);
	}

	// ==================== VkRaytracingPipeline ====================

	VkRaytracingPipeline::VkRaytracingPipeline(VkGraphics* graphics)
		: m_Graphics(graphics)
	{
	}

	VkRaytracingPipeline::~VkRaytracingPipeline() noexcept
	{
		auto device = m_Graphics->GetDevice();

		if (m_Pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(device, m_Pipeline, nullptr);
		}

		if (m_PipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
		}

		if (m_DescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(device, m_DescriptorPool, nullptr);
		}

		if (m_DescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
		}

		if (m_SbtBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device, m_SbtBuffer, nullptr);
		}

		if (m_SbtMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device, m_SbtMemory, nullptr);
		}
	}

	VkShaderModule VkRaytracingPipeline::CreateShaderModule(const std::vector<uint32_t>& code)
	{
		VkShaderModuleCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = code.size() * sizeof(uint32_t),
			.pCode = code.data()
		};

		VkShaderModule shaderModule;
		VKR(vkCreateShaderModule(m_Graphics->GetDevice(), &createInfo, nullptr, &shaderModule));
		return shaderModule;
	}

	void VkRaytracingPipeline::CreatePipeline()
	{
		auto device = m_Graphics->GetDevice();

		// Create descriptor set layout
		VkDescriptorSetLayoutBinding asBinding
		{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR
		};

		VkDescriptorSetLayoutBinding imageBinding
		{
			.binding = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR
		};

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { asBinding, imageBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = (uint32_t)bindings.size(),
			.pBindings = bindings.data()
		};

		VKR(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayout));

		// Create pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 1,
			.pSetLayouts = &m_DescriptorSetLayout
		};

		VKR(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout));

		// TODO: Replace hard-coded SPIR-V with proper shader compilation system or load from .spv files
		// These are minimal valid shaders for testing
		
		// Ray Generation Shader - writes red color to output image
		// #version 460
		// #extension GL_EXT_ray_tracing : require
		// layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
		// layout(binding = 1, set = 0, rgba8) uniform image2D image;
		// layout(location = 0) rayPayloadEXT vec3 hitValue;
		// void main() {
		//     vec2 pixelCenter = vec2(gl_LaunchIDEXT.xy) + vec2(0.5);
		//     vec2 inUV = pixelCenter/vec2(gl_LaunchSizeEXT.xy);
		//     vec2 d = inUV * 2.0 - 1.0;
		//     vec3 origin = vec3(0.0, 0.0, -2.0);
		//     vec3 direction = normalize(vec3(d.x, -d.y, 1.0));
		//     hitValue = vec3(0.0, 0.0, 0.0);
		//     traceRayEXT(topLevelAS, gl_RayFlagsOpaqueEXT, 0xff, 0, 0, 0, origin, 0.001, direction, 10000.0, 0);
		//     imageStore(image, ivec2(gl_LaunchIDEXT.xy), vec4(hitValue, 1.0));
		// }
		std::vector<uint32_t> raygenCode = {
			0x07230203, 0x00010500, 0x00080007, 0x0000005e, 0x00000000, 0x00020011, 0x00000001, 0x00020011,
			0x00003542, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e,
			0x00000000, 0x00000001, 0x0007000f, 0x00000005, 0x00000004, 0x6e69616d, 0x00000000, 0x0000000b,
			0x0000003b, 0x00030010, 0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001cc, 0x00040005,
			0x00000004, 0x6e69616d, 0x00000000, 0x00050005, 0x00000009, 0x65786970, 0x6e65436c, 0x00726574,
			0x00060005, 0x0000000b, 0x4c5f6c67, 0x636e7561, 0x44494868, 0x00545845, 0x00030005, 0x00000011,
			0x00565569, 0x00050005, 0x00000016, 0x676e6972, 0x6567726f, 0x0000006e, 0x00060005, 0x0000001b,
			0x65636164, 0x6f72656c, 0x6f6e6974, 0x0000006e, 0x00060005, 0x00000023, 0x5674696e, 0x65756c61,
			0x00000000, 0x00060005, 0x00000026, 0x4c706f74, 0x6c657665, 0x00004741, 0x00040005, 0x0000002e,
			0x67616d69, 0x00000065, 0x00060005, 0x0000003b, 0x4c5f6c67, 0x636e7561, 0x7a695368, 0x00544578,
			0x00050048, 0x00000009, 0x00000000, 0x00000023, 0x00000000, 0x00030047, 0x00000009, 0x00000002,
			0x00040047, 0x0000000b, 0x0000000b, 0x0000001c, 0x00040047, 0x00000023, 0x0000001e, 0x00000000,
			0x00040047, 0x00000026, 0x00000022, 0x00000000, 0x00040047, 0x00000026, 0x00000021, 0x00000000,
			0x00040047, 0x0000002e, 0x00000022, 0x00000000, 0x00040047, 0x0000002e, 0x00000021, 0x00000001,
			0x00030047, 0x0000002e, 0x00000019, 0x00040047, 0x0000003b, 0x0000000b, 0x0000001b, 0x00020013,
			0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017,
			0x00000007, 0x00000006, 0x00000002, 0x00040020, 0x00000008, 0x00000007, 0x00000007, 0x00040015,
			0x0000000a, 0x00000020, 0x00000000, 0x00040017, 0x0000000c, 0x0000000a, 0x00000003, 0x00040020,
			0x0000000d, 0x00000001, 0x0000000c, 0x0004003b, 0x0000000d, 0x0000000b, 0x00000001, 0x00040017,
			0x0000000e, 0x0000000a, 0x00000002, 0x0004002b, 0x00000006, 0x00000012, 0x3f000000, 0x00040017,
			0x00000013, 0x00000006, 0x00000003, 0x00040020, 0x00000014, 0x00000007, 0x00000013, 0x0004002b,
			0x00000006, 0x00000017, 0x40000000, 0x0004002b, 0x00000006, 0x00000018, 0x3f800000, 0x0004002b,
			0x00000006, 0x0000001c, 0x00000000, 0x0004002b, 0x00000006, 0x0000001d, 0xc0000000, 0x0006002c,
			0x00000013, 0x0000001e, 0x0000001c, 0x0000001c, 0x0000001d, 0x00040020, 0x00000022, 0x00000003,
			0x00000013, 0x0004003b, 0x00000022, 0x00000023, 0x00000003, 0x00090019, 0x00000024, 0x00000006,
			0x00000008, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x00040020, 0x00000025,
			0x00000000, 0x00000024, 0x0004003b, 0x00000025, 0x00000026, 0x00000000, 0x0004002b, 0x0000000a,
			0x00000027, 0x000000ff, 0x0004002b, 0x0000000a, 0x00000028, 0x00000000, 0x0004002b, 0x00000006,
			0x00000029, 0x3a83126f, 0x0004002b, 0x00000006, 0x0000002a, 0x461c4000, 0x00090019, 0x0000002c,
			0x00000006, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000008, 0x00040020,
			0x0000002d, 0x00000000, 0x0000002c, 0x0004003b, 0x0000002d, 0x0000002e, 0x00000000, 0x00040017,
			0x00000030, 0x0000000a, 0x00000002, 0x00040017, 0x00000033, 0x00000006, 0x00000004, 0x0004003b,
			0x0000000d, 0x0000003b, 0x00000001, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003,
			0x000200f8, 0x00000005, 0x0004003b, 0x00000008, 0x00000009, 0x00000007, 0x0004003b, 0x00000008,
			0x00000011, 0x00000007, 0x0004003b, 0x00000014, 0x00000016, 0x00000007, 0x0004003b, 0x00000014,
			0x0000001b, 0x00000007, 0x0004003d, 0x0000000c, 0x0000000f, 0x0000000b, 0x0007004f, 0x0000000e,
			0x00000010, 0x0000000f, 0x0000000f, 0x00000000, 0x00000001, 0x00040070, 0x00000007, 0x00000015,
			0x00000010, 0x00050081, 0x00000007, 0x00000019, 0x00000015, 0x00000012, 0x0003003e, 0x00000009,
			0x00000019, 0x0004003d, 0x00000007, 0x0000001a, 0x00000009, 0x0004003d, 0x0000000c, 0x0000001f,
			0x0000003b, 0x0007004f, 0x0000000e, 0x00000020, 0x0000001f, 0x0000001f, 0x00000000, 0x00000001,
			0x00040070, 0x00000007, 0x00000021, 0x00000020, 0x00050085, 0x00000007, 0x0000002f, 0x0000001a,
			0x00000021, 0x00050083, 0x00000007, 0x00000031, 0x0000002f, 0x00000018, 0x0003003e, 0x00000011,
			0x00000031, 0x0004003d, 0x00000007, 0x00000032, 0x00000011, 0x00050051, 0x00000006, 0x00000034,
			0x00000032, 0x00000000, 0x00050051, 0x00000006, 0x00000035, 0x00000032, 0x00000001, 0x00060050,
			0x00000013, 0x00000036, 0x00000034, 0x00000035, 0x00000018, 0x0006000c, 0x00000013, 0x00000037,
			0x00000001, 0x00000045, 0x00000036, 0x0003003e, 0x0000001b, 0x00000037, 0x0003003e, 0x00000016,
			0x0000001e, 0x0006003e, 0x00000023, 0x0000001c, 0x00000026, 0x00000027, 0x00000028, 0x0004003d,
			0x00000024, 0x00000038, 0x00000026, 0x0004003d, 0x00000013, 0x00000039, 0x00000016, 0x0004003d,
			0x00000013, 0x0000003a, 0x0000001b, 0x0008003d, 0x00000002, 0x0000003c, 0x00000038, 0x00000039,
			0x00000029, 0x0000003a, 0x0000002a, 0x0004003d, 0x0000002c, 0x0000003e, 0x0000002e, 0x0004003d,
			0x0000000c, 0x0000003f, 0x0000000b, 0x0007004f, 0x0000000e, 0x00000040, 0x0000003f, 0x0000003f,
			0x00000000, 0x00000001, 0x0004006e, 0x00000030, 0x00000041, 0x00000040, 0x0004003d, 0x00000013,
			0x00000042, 0x00000023, 0x00050051, 0x00000006, 0x00000043, 0x00000042, 0x00000000, 0x00050051,
			0x00000006, 0x00000044, 0x00000042, 0x00000001, 0x00050051, 0x00000006, 0x00000045, 0x00000042,
			0x00000002, 0x00070050, 0x00000033, 0x00000046, 0x00000043, 0x00000044, 0x00000045, 0x00000018,
			0x00040063, 0x0000003e, 0x00000041, 0x00000046, 0x000100fd, 0x00010038
		};

		// Miss Shader - returns background color (black)
		// #version 460
		// #extension GL_EXT_ray_tracing : require
		// layout(location = 0) rayPayloadInEXT vec3 hitValue;
		// void main() {
		//     hitValue = vec3(0.0, 0.0, 0.0);
		// }
		std::vector<uint32_t> missCode = {
			0x07230203, 0x00010500, 0x00080005, 0x0000000d, 0x00000000, 0x00020011, 0x00000001, 0x00020011,
			0x00003542, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e,
			0x00000000, 0x00000001, 0x0006000f, 0x00000006, 0x00000004, 0x6e69616d, 0x00000000, 0x00000009,
			0x00030010, 0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001cc, 0x00040005, 0x00000004,
			0x6e69616d, 0x00000000, 0x00060005, 0x00000009, 0x56746968, 0x65756c61, 0x00000000, 0x00030047,
			0x00000009, 0x00000000, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00020013, 0x00000002,
			0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007,
			0x00000006, 0x00000003, 0x00040020, 0x00000008, 0x00000003, 0x00000007, 0x0004003b, 0x00000008,
			0x00000009, 0x00000003, 0x0004002b, 0x00000006, 0x0000000a, 0x00000000, 0x0006002c, 0x00000007,
			0x0000000b, 0x0000000a, 0x0000000a, 0x0000000a, 0x00050036, 0x00000002, 0x00000004, 0x00000000,
			0x00000003, 0x000200f8, 0x00000005, 0x0003003e, 0x00000009, 0x0000000b, 0x000100fd, 0x00010038
		};

		// Closest Hit Shader - returns green color for triangle
		// #version 460
		// #extension GL_EXT_ray_tracing : require
		// layout(location = 0) rayPayloadInEXT vec3 hitValue;
		// void main() {
		//     hitValue = vec3(0.0, 1.0, 0.0);
		// }
		std::vector<uint32_t> chitCode = {
			0x07230203, 0x00010500, 0x00080005, 0x0000000d, 0x00000000, 0x00020011, 0x00000001, 0x00020011,
			0x00003542, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e,
			0x00000000, 0x00000001, 0x0006000f, 0x00000008, 0x00000004, 0x6e69616d, 0x00000000, 0x00000009,
			0x00030010, 0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001cc, 0x00040005, 0x00000004,
			0x6e69616d, 0x00000000, 0x00060005, 0x00000009, 0x56746968, 0x65756c61, 0x00000000, 0x00030047,
			0x00000009, 0x00000000, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00020013, 0x00000002,
			0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007,
			0x00000006, 0x00000003, 0x00040020, 0x00000008, 0x00000003, 0x00000007, 0x0004003b, 0x00000008,
			0x00000009, 0x00000003, 0x0004002b, 0x00000006, 0x0000000a, 0x00000000, 0x0004002b, 0x00000006,
			0x0000000b, 0x3f800000, 0x0006002c, 0x00000007, 0x0000000c, 0x0000000a, 0x0000000b, 0x0000000a,
			0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0003003e,
			0x00000009, 0x0000000c, 0x000100fd, 0x00010038
		};

		VkShaderModule raygenModule = CreateShaderModule(raygenCode);
		VkShaderModule missModule = CreateShaderModule(missCode);
		VkShaderModule chitModule = CreateShaderModule(chitCode);

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		// Raygen stage
		shaderStages.push_back(VkPipelineShaderStageCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_RAYGEN_BIT_KHR,
			.module = raygenModule,
			.pName = "main"
		});

		// Miss stage
		shaderStages.push_back(VkPipelineShaderStageCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_MISS_BIT_KHR,
			.module = missModule,
			.pName = "main"
		});

		// Closest hit stage
		shaderStages.push_back(VkPipelineShaderStageCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.stage = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
			.module = chitModule,
			.pName = "main"
		});

		std::vector<VkRayTracingShaderGroupCreateInfoKHR> shaderGroups;

		// Raygen group
		shaderGroups.push_back(VkRayTracingShaderGroupCreateInfoKHR
		{
			.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
			.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR,
			.generalShader = 0,
			.closestHitShader = VK_SHADER_UNUSED_KHR,
			.anyHitShader = VK_SHADER_UNUSED_KHR,
			.intersectionShader = VK_SHADER_UNUSED_KHR
		});

		// Miss group
		shaderGroups.push_back(VkRayTracingShaderGroupCreateInfoKHR
		{
			.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
			.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR,
			.generalShader = 1,
			.closestHitShader = VK_SHADER_UNUSED_KHR,
			.anyHitShader = VK_SHADER_UNUSED_KHR,
			.intersectionShader = VK_SHADER_UNUSED_KHR
		});

		// Hit group
		shaderGroups.push_back(VkRayTracingShaderGroupCreateInfoKHR
		{
			.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
			.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR,
			.generalShader = VK_SHADER_UNUSED_KHR,
			.closestHitShader = 2,
			.anyHitShader = VK_SHADER_UNUSED_KHR,
			.intersectionShader = VK_SHADER_UNUSED_KHR
		});

		VkRayTracingPipelineCreateInfoKHR pipelineInfo
		{
			.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR,
			.stageCount = (uint32_t)shaderStages.size(),
			.pStages = shaderStages.data(),
			.groupCount = (uint32_t)shaderGroups.size(),
			.pGroups = shaderGroups.data(),
			.maxPipelineRayRecursionDepth = 1,
			.layout = m_PipelineLayout
		};

		auto vkCreateRayTracingPipelinesKHR = (PFN_vkCreateRayTracingPipelinesKHR)
			vkGetDeviceProcAddr(device, "vkCreateRayTracingPipelinesKHR");

		VKR(vkCreateRayTracingPipelinesKHR(device, VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline));

		// Cleanup shader modules
		vkDestroyShaderModule(device, raygenModule, nullptr);
		vkDestroyShaderModule(device, missModule, nullptr);
		vkDestroyShaderModule(device, chitModule, nullptr);

		// Create descriptor pool
		std::array<VkDescriptorPoolSize, 2> poolSizes = {
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1 },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1 }
		};

		VkDescriptorPoolCreateInfo poolInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.maxSets = 1,
			.poolSizeCount = (uint32_t)poolSizes.size(),
			.pPoolSizes = poolSizes.data()
		};

		VKR(vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_DescriptorPool));

		// Allocate descriptor set
		VkDescriptorSetAllocateInfo allocInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = m_DescriptorPool,
			.descriptorSetCount = 1,
			.pSetLayouts = &m_DescriptorSetLayout
		};

		VKR(vkAllocateDescriptorSets(device, &allocInfo, &m_DescriptorSet));
	}

	void VkRaytracingPipeline::CreateShaderBindingTable()
	{
		auto device = m_Graphics->GetDevice();

		auto vkGetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR)
			vkGetDeviceProcAddr(device, "vkGetRayTracingShaderGroupHandlesKHR");

		VkPhysicalDeviceRayTracingPipelinePropertiesKHR rtProps
		{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR
		};

		VkPhysicalDeviceProperties2 props2
		{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
			.pNext = &rtProps
		};

		vkGetPhysicalDeviceProperties2(m_Graphics->GetPhysicalDevice(), &props2);

		uint32_t handleSize = rtProps.shaderGroupHandleSize;
		uint32_t handleSizeAligned = (handleSize + rtProps.shaderGroupHandleAlignment - 1) & ~(rtProps.shaderGroupHandleAlignment - 1);
		uint32_t groupCount = 3; // raygen, miss, hit

		uint32_t sbtSize = groupCount * handleSizeAligned;

		std::vector<uint8_t> shaderHandles(groupCount * handleSize);
		VKR(vkGetRayTracingShaderGroupHandlesKHR(device, m_Pipeline, 0, groupCount, shaderHandles.size(), shaderHandles.data()));

		// Create SBT buffer
		VkBufferCreateInfo bufferInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sbtSize,
			.usage = VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};

		VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &m_SbtBuffer));

		VkMemoryRequirements memReq;
		vkGetBufferMemoryRequirements(device, m_SbtBuffer, &memReq);

		VkPhysicalDeviceMemoryProperties memProps;
		vkGetPhysicalDeviceMemoryProperties(m_Graphics->GetPhysicalDevice(), &memProps);

		uint32_t memoryTypeIndex = UINT32_MAX;
		for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i)
		{
			if ((memReq.memoryTypeBits & (1u << i)) &&
				(memProps.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)))
			{
				memoryTypeIndex = i;
				break;
			}
		}

		VkMemoryAllocateFlagsInfo allocFlags
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
			.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT
		};

		VkMemoryAllocateInfo allocInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = &allocFlags,
			.allocationSize = memReq.size,
			.memoryTypeIndex = memoryTypeIndex
		};

		VKR(vkAllocateMemory(device, &allocInfo, nullptr, &m_SbtMemory));
		VKR(vkBindBufferMemory(device, m_SbtBuffer, m_SbtMemory, 0));

		// Map and copy handles
		void* mapped;
		VKR(vkMapMemory(device, m_SbtMemory, 0, sbtSize, 0, &mapped));
		
		auto* dst = static_cast<uint8_t*>(mapped);
		for (uint32_t i = 0; i < groupCount; ++i)
		{
			std::memcpy(dst + i * handleSizeAligned, shaderHandles.data() + i * handleSize, handleSize);
		}
		
		vkUnmapMemory(device, m_SbtMemory);

		// Get SBT buffer address
		VkBufferDeviceAddressInfo addrInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
			.buffer = m_SbtBuffer
		};
		VkDeviceAddress sbtAddress = vkGetBufferDeviceAddress(device, &addrInfo);

		// Setup regions
		m_RaygenRegion = VkStridedDeviceAddressRegionKHR
		{
			.deviceAddress = sbtAddress,
			.stride = handleSizeAligned,
			.size = handleSizeAligned
		};

		m_MissRegion = VkStridedDeviceAddressRegionKHR
		{
			.deviceAddress = sbtAddress + handleSizeAligned,
			.stride = handleSizeAligned,
			.size = handleSizeAligned
		};

		m_HitRegion = VkStridedDeviceAddressRegionKHR
		{
			.deviceAddress = sbtAddress + 2 * handleSizeAligned,
			.stride = handleSizeAligned,
			.size = handleSizeAligned
		};

		m_CallableRegion = VkStridedDeviceAddressRegionKHR{};
	}

	void VkRaytracingPipeline::UpdateDescriptorSet(VkAccelerationStructureKHR tlas, VkImageView outputImageView)
	{
		auto device = m_Graphics->GetDevice();

		// Update AS descriptor
		VkWriteDescriptorSetAccelerationStructureKHR asDescriptor
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR,
			.accelerationStructureCount = 1,
			.pAccelerationStructures = &tlas
		};

		VkWriteDescriptorSet asWrite
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = &asDescriptor,
			.dstSet = m_DescriptorSet,
			.dstBinding = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR
		};

		// Update image descriptor
		VkDescriptorImageInfo imageInfo
		{
			.imageView = outputImageView,
			.imageLayout = VK_IMAGE_LAYOUT_GENERAL
		};

		VkWriteDescriptorSet imageWrite
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_DescriptorSet,
			.dstBinding = 1,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
			.pImageInfo = &imageInfo
		};

		std::array<VkWriteDescriptorSet, 2> writes = { asWrite, imageWrite };
		vkUpdateDescriptorSets(device, (uint32_t)writes.size(), writes.data(), 0, nullptr);
	}
}
