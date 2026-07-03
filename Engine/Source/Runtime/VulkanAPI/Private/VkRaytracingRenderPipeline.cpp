// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkRaytracingRenderPipeline.h"
#include "VkCommandBuffer.h"
#include "VkGraphics.h"
#include "VkShader.h"
#include "VkSwapchainRenderTexture.h"
#include "Rendering/ShaderType.h"

namespace Ayla
{
	namespace
	{
		VkDeviceSize AlignUp(VkDeviceSize value, VkDeviceSize alignment) noexcept
		{
			return alignment > 0 ? ((value + alignment - 1) / alignment) * alignment : value;
		}

		struct TemporaryShaderModule
		{
			VkDevice m_Device = VK_NULL_HANDLE;
			VkShaderModule m_Module = VK_NULL_HANDLE;

			TemporaryShaderModule(VkDevice device, const ShaderBytecode& bytecode)
				: m_Device(device)
			{
				if (bytecode.Bytecode.empty())
				{
					throw InvalidOperationException(TEXT("Cannot create a Vulkan shader module from empty shader bytecode."));
				}

				VkShaderModuleCreateInfo moduleInfo
				{
					.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
					.codeSize = bytecode.Bytecode.size(),
					.pCode = reinterpret_cast<const uint32_t*>(bytecode.Bytecode.data()),
				};

				VKR(vkCreateShaderModule(device, &moduleInfo, nullptr, &m_Module));
			}

			TemporaryShaderModule(const TemporaryShaderModule&) = delete;
			TemporaryShaderModule& operator=(const TemporaryShaderModule&) = delete;

			TemporaryShaderModule(TemporaryShaderModule&& rhs) noexcept
				: m_Device(rhs.m_Device)
				, m_Module(rhs.m_Module)
			{
				rhs.m_Device = VK_NULL_HANDLE;
				rhs.m_Module = VK_NULL_HANDLE;
			}

			TemporaryShaderModule& operator=(TemporaryShaderModule&& rhs) noexcept
			{
				Reset();
				m_Device = rhs.m_Device;
				m_Module = rhs.m_Module;
				rhs.m_Device = VK_NULL_HANDLE;
				rhs.m_Module = VK_NULL_HANDLE;
				return *this;
			}

			~TemporaryShaderModule() noexcept
			{
				Reset();
			}

			void Reset() noexcept
			{
				if (m_Module != VK_NULL_HANDLE)
				{
					vkDestroyShaderModule(m_Device, m_Module, nullptr);
					m_Module = VK_NULL_HANDLE;
				}
			}
		};

		void ValidateRequiredShader(const Shader& shader, ShaderType shaderType, String shaderName)
		{
			if (shader.Has(shaderType) == false)
			{
				throw InvalidOperationException(String::Format(
					TEXT("Vulkan raytracing render pipeline requires a {} shader."),
					shaderName
				));
			}
		}

		uint32 AppendShaderStage(
			VkDevice device,
			const Shader& shader,
			ShaderType shaderType,
			VkShaderStageFlagBits stage,
			std::vector<TemporaryShaderModule>& modules,
			std::vector<std::string>& entrypointNames,
			std::vector<VkPipelineShaderStageCreateInfo>& shaderStages)
		{
			const ShaderBytecode& bytecode = shader.GetBytecode(shaderType);
			if (bytecode.EntrypointName.IsEmpty())
			{
				throw InvalidOperationException(TEXT("Vulkan raytracing shader bytecode is missing an entrypoint name."));
			}

			TemporaryShaderModule& module = modules.emplace_back(device, bytecode);
			std::string& entrypointName = entrypointNames.emplace_back(bytecode.EntrypointName.AsCodepage());

			shaderStages.emplace_back(VkPipelineShaderStageCreateInfo
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = stage,
				.module = module.m_Module,
				.pName = entrypointName.c_str(),
			});

			return static_cast<uint32>(shaderStages.size() - 1);
		}

		VkRayTracingShaderGroupCreateInfoKHR MakeGeneralShaderGroup(uint32 shaderIndex)
		{
			return VkRayTracingShaderGroupCreateInfoKHR
			{
				.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
				.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR,
				.generalShader = shaderIndex,
				.closestHitShader = VK_SHADER_UNUSED_KHR,
				.anyHitShader = VK_SHADER_UNUSED_KHR,
				.intersectionShader = VK_SHADER_UNUSED_KHR,
			};
		}

		VkRayTracingShaderGroupCreateInfoKHR MakeTrianglesHitShaderGroup(uint32 closestHitShaderIndex, uint32 anyHitShaderIndex)
		{
			return VkRayTracingShaderGroupCreateInfoKHR
			{
				.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
				.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR,
				.generalShader = VK_SHADER_UNUSED_KHR,
				.closestHitShader = closestHitShaderIndex,
				.anyHitShader = anyHitShaderIndex,
				.intersectionShader = VK_SHADER_UNUSED_KHR,
			};
		}

		VkDeviceAddress GetBufferDeviceAddress(VkGraphics* graphics, ::VkBuffer buffer)
		{
			VkBufferDeviceAddressInfo addressInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
				.buffer = buffer,
			};

			return graphics->GetBufferDeviceAddressFunction()(graphics->GetDevice(), &addressInfo);
		}
	}

	VkRaytracingRenderPipeline::VkRaytracingRenderPipeline(VkGraphics* graphics, SharedPtr<Shader> shader)
		: m_Graphics(graphics)
	{
		Shader* shaderPtr = shader.Get();
		if (shaderPtr == nullptr)
		{
			throw InvalidOperationException(TEXT("Vulkan raytracing render pipeline requires a shader."));
		}

		ValidateRequiredShader(*shaderPtr, ShaderType::RayGeneration, TEXT("ray generation"));
		ValidateRequiredShader(*shaderPtr, ShaderType::Miss, TEXT("miss"));

		auto device = graphics->GetDevice();

		std::array<VkDescriptorSetLayoutBinding, 2> descriptorBindings =
		{
			VkDescriptorSetLayoutBinding
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR,
			},
			VkDescriptorSetLayoutBinding
			{
				.binding = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR,
			},
		};

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = static_cast<uint32>(descriptorBindings.size()),
			.pBindings = descriptorBindings.data(),
		};

		VKR(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutInfo, nullptr, &m_DescriptorSetLayout));

		VkPipelineLayoutCreateInfo pipelineLayoutInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 1,
			.pSetLayouts = &m_DescriptorSetLayout,
		};

		VKR(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout));
		CreateDescriptorSets();

		std::vector<TemporaryShaderModule> modules;
		modules.reserve(4);

		std::vector<std::string> entrypointNames;
		entrypointNames.reserve(4);

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		shaderStages.reserve(4);

		std::vector<VkRayTracingShaderGroupCreateInfoKHR> shaderGroups;
		shaderGroups.reserve(3);

		uint32 rayGenerationShaderIndex = AppendShaderStage(
			device,
			*shaderPtr,
			ShaderType::RayGeneration,
			VK_SHADER_STAGE_RAYGEN_BIT_KHR,
			modules,
			entrypointNames,
			shaderStages);

		m_RayGenerationGroupIndex = static_cast<uint32>(shaderGroups.size());
		shaderGroups.emplace_back(MakeGeneralShaderGroup(rayGenerationShaderIndex));

		uint32 missShaderIndex = AppendShaderStage(
			device,
			*shaderPtr,
			ShaderType::Miss,
			VK_SHADER_STAGE_MISS_BIT_KHR,
			modules,
			entrypointNames,
			shaderStages);

		m_MissGroupIndex = static_cast<uint32>(shaderGroups.size());
		shaderGroups.emplace_back(MakeGeneralShaderGroup(missShaderIndex));

		uint32 closestHitShaderIndex = VK_SHADER_UNUSED_KHR;
		if (shaderPtr->Has(ShaderType::ClosestHit))
		{
			closestHitShaderIndex = AppendShaderStage(
				device,
				*shaderPtr,
				ShaderType::ClosestHit,
				VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
				modules,
				entrypointNames,
				shaderStages);
		}

		uint32 anyHitShaderIndex = VK_SHADER_UNUSED_KHR;
		if (shaderPtr->Has(ShaderType::AnyHit))
		{
			anyHitShaderIndex = AppendShaderStage(
				device,
				*shaderPtr,
				ShaderType::AnyHit,
				VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
				modules,
				entrypointNames,
				shaderStages);
		}

		if (closestHitShaderIndex != VK_SHADER_UNUSED_KHR || anyHitShaderIndex != VK_SHADER_UNUSED_KHR)
		{
			m_HitGroupIndex = static_cast<uint32>(shaderGroups.size());
			shaderGroups.emplace_back(MakeTrianglesHitShaderGroup(closestHitShaderIndex, anyHitShaderIndex));
		}

		VkRayTracingPipelineCreateInfoKHR pipelineInfo
		{
			.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR,
			.stageCount = static_cast<uint32>(shaderStages.size()),
			.pStages = shaderStages.data(),
			.groupCount = static_cast<uint32>(shaderGroups.size()),
			.pGroups = shaderGroups.data(),
			.maxPipelineRayRecursionDepth = 1,
			.layout = m_PipelineLayout,
		};

		VKR(graphics->GetCreateRayTracingPipelinesKHRFunction()(device, VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline));
		CreateShaderBindingTable(static_cast<uint32>(shaderGroups.size()));
	}

	VkRaytracingRenderPipeline::~VkRaytracingRenderPipeline() noexcept
	{
		auto device = m_Graphics->GetDevice();

		if (m_Pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(device, m_Pipeline, nullptr);
		}
		if (m_ShaderBindingTableBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device, m_ShaderBindingTableBuffer, nullptr);
		}
		if (m_ShaderBindingTableMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device, m_ShaderBindingTableMemory, nullptr);
		}
		if (m_DescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(device, m_DescriptorPool, nullptr);
		}
		if (m_PipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
		}
		if (m_DescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
		}
	}

	void VkRaytracingRenderPipeline::SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset)
	{
	}

	void VkRaytracingRenderPipeline::BindOutputTexture(VkCommandBuffer* cmd, VkSwapchainRenderTexture* renderTexture)
	{
		auto descriptorSet = m_DescriptorSets[m_Graphics->GetFrameIndex()];
		VkDescriptorImageInfo imageInfo
		{
			.imageView = renderTexture->GetCurrentImageView(),
			.imageLayout = VK_IMAGE_LAYOUT_GENERAL,
		};

		VkWriteDescriptorSet write
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = descriptorSet,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
			.pImageInfo = &imageInfo,
		};

		vkUpdateDescriptorSets(m_Graphics->GetDevice(), 1, &write, 0, nullptr);
		vkCmdBindDescriptorSets(
			cmd->GetVkCommandBuffer(),
			VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
			m_PipelineLayout,
			0,
			1,
			&descriptorSet,
			0,
			nullptr
		);
	}

	void VkRaytracingRenderPipeline::CreateDescriptorSets()
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes =
		{
			VkDescriptorPoolSize
			{
				.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
				.descriptorCount = static_cast<uint32>(Graphics::kMaxFramesInFlight),
			},
			VkDescriptorPoolSize
			{
				.type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
				.descriptorCount = static_cast<uint32>(Graphics::kMaxFramesInFlight),
			},
		};

		VkDescriptorPoolCreateInfo poolInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.maxSets = static_cast<uint32>(Graphics::kMaxFramesInFlight),
			.poolSizeCount = static_cast<uint32>(poolSizes.size()),
			.pPoolSizes = poolSizes.data(),
		};

		VKR(vkCreateDescriptorPool(m_Graphics->GetDevice(), &poolInfo, nullptr, &m_DescriptorPool));

		std::vector<VkDescriptorSetLayout> layouts(Graphics::kMaxFramesInFlight, m_DescriptorSetLayout);
		VkDescriptorSetAllocateInfo allocateInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = m_DescriptorPool,
			.descriptorSetCount = static_cast<uint32>(layouts.size()),
			.pSetLayouts = layouts.data(),
		};

		m_DescriptorSets.resize(Graphics::kMaxFramesInFlight);
		VKR(vkAllocateDescriptorSets(m_Graphics->GetDevice(), &allocateInfo, m_DescriptorSets.data()));
	}

	void VkRaytracingRenderPipeline::CreateShaderBindingTable(uint32 shaderGroupCount)
	{
		auto device = m_Graphics->GetDevice();
		const auto& properties = m_Graphics->GetRaytracingPipelineProperties();

		const uint32 shaderGroupHandleSize = properties.shaderGroupHandleSize;
		const VkDeviceSize shaderGroupHandleAlignment = properties.shaderGroupHandleAlignment;
		const VkDeviceSize shaderGroupBaseAlignment = properties.shaderGroupBaseAlignment;
		const VkDeviceSize shaderGroupHandleSizeAligned = AlignUp(shaderGroupHandleSize, shaderGroupHandleAlignment);

		m_RayGenerationShaderBindingTable.stride = AlignUp(shaderGroupHandleSizeAligned, shaderGroupBaseAlignment);
		m_RayGenerationShaderBindingTable.size = m_RayGenerationShaderBindingTable.stride;

		m_MissShaderBindingTable.stride = shaderGroupHandleSizeAligned;
		m_MissShaderBindingTable.size = AlignUp(shaderGroupHandleSizeAligned, shaderGroupBaseAlignment);

		if (m_HitGroupIndex != VK_SHADER_UNUSED_KHR)
		{
			m_HitShaderBindingTable.stride = shaderGroupHandleSizeAligned;
			m_HitShaderBindingTable.size = AlignUp(shaderGroupHandleSizeAligned, shaderGroupBaseAlignment);
		}

		const VkDeviceSize rayGenerationOffset = 0;
		const VkDeviceSize missOffset = rayGenerationOffset + m_RayGenerationShaderBindingTable.size;
		const VkDeviceSize hitOffset = missOffset + m_MissShaderBindingTable.size;
		const VkDeviceSize shaderBindingTableSize = hitOffset + m_HitShaderBindingTable.size;

		std::vector<uint8> shaderGroupHandles(static_cast<size_t>(shaderGroupCount) * shaderGroupHandleSize);
		VKR(m_Graphics->GetRayTracingShaderGroupHandlesKHRFunction()(
			device,
			m_Pipeline,
			0,
			shaderGroupCount,
			shaderGroupHandles.size(),
			shaderGroupHandles.data()
		));

		VkBufferCreateInfo bufferInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = shaderBindingTableSize,
			.usage = VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		};

		VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &m_ShaderBindingTableBuffer));

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(device, m_ShaderBindingTableBuffer, &memoryRequirements);

		uint32 memoryTypeIndex = m_Graphics->FindMemoryType(
			memoryRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		if (memoryTypeIndex == UINT32_MAX)
		{
			throw InvalidOperationException(TEXT("Could not find host visible memory for the Vulkan shader binding table."));
		}

		VkMemoryAllocateFlagsInfo allocateFlagsInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
			.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT,
		};

		VkMemoryAllocateInfo allocateInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = &allocateFlagsInfo,
			.allocationSize = memoryRequirements.size,
			.memoryTypeIndex = memoryTypeIndex,
		};

		VKR(vkAllocateMemory(device, &allocateInfo, nullptr, &m_ShaderBindingTableMemory));
		VKR(vkBindBufferMemory(device, m_ShaderBindingTableBuffer, m_ShaderBindingTableMemory, 0));

		void* mappedData = nullptr;
		VKR(vkMapMemory(device, m_ShaderBindingTableMemory, 0, shaderBindingTableSize, 0, &mappedData));
		std::memset(mappedData, 0, static_cast<size_t>(shaderBindingTableSize));

		auto copyShaderGroupHandle = [&](uint32 shaderGroupIndex, VkDeviceSize destinationOffset)
		{
			if (shaderGroupIndex == VK_SHADER_UNUSED_KHR)
			{
				return;
			}

			const uint8* source = shaderGroupHandles.data() + static_cast<size_t>(shaderGroupIndex) * shaderGroupHandleSize;
			uint8* destination = static_cast<uint8*>(mappedData) + destinationOffset;
			std::memcpy(destination, source, shaderGroupHandleSize);
		};

		copyShaderGroupHandle(m_RayGenerationGroupIndex, rayGenerationOffset);
		copyShaderGroupHandle(m_MissGroupIndex, missOffset);
		copyShaderGroupHandle(m_HitGroupIndex, hitOffset);
		vkUnmapMemory(device, m_ShaderBindingTableMemory);

		m_ShaderBindingTableAddress = GetBufferDeviceAddress(m_Graphics, m_ShaderBindingTableBuffer);
		m_RayGenerationShaderBindingTable.deviceAddress = m_ShaderBindingTableAddress + rayGenerationOffset;
		m_MissShaderBindingTable.deviceAddress = m_ShaderBindingTableAddress + missOffset;
		if (m_HitShaderBindingTable.size > 0)
		{
			m_HitShaderBindingTable.deviceAddress = m_ShaderBindingTableAddress + hitOffset;
		}
	}
}
