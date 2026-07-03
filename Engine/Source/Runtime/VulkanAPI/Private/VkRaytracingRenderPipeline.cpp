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

		struct RaytracingVertex
		{
			float m_X;
			float m_Y;
			float m_Z;
		};

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

		VkDeviceAddress GetVkBufferDeviceAddress(VkGraphics* graphics, ::VkBuffer buffer)
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
		CreateTestTriangleAccelerationStructures();
	}

	VkRaytracingRenderPipeline::~VkRaytracingRenderPipeline() noexcept
	{
		auto device = m_Graphics->GetDevice();

		if (m_Pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(device, m_Pipeline, nullptr);
		}
		if (m_TopLevelAccelerationStructure != VK_NULL_HANDLE)
		{
			m_Graphics->GetDestroyAccelerationStructureKHRFunction()(device, m_TopLevelAccelerationStructure, nullptr);
		}
		if (m_BottomLevelAccelerationStructure != VK_NULL_HANDLE)
		{
			m_Graphics->GetDestroyAccelerationStructureKHRFunction()(device, m_BottomLevelAccelerationStructure, nullptr);
		}
		DestroyBuffer(&m_ScratchBuffer);
		DestroyBuffer(&m_InstanceBuffer);
		DestroyBuffer(&m_TopLevelAccelerationStructureBuffer);
		DestroyBuffer(&m_BottomLevelAccelerationStructureBuffer);
		DestroyBuffer(&m_TriangleIndexBuffer);
		DestroyBuffer(&m_TriangleVertexBuffer);
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

		VkWriteDescriptorSetAccelerationStructureKHR accelerationStructureInfo
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR,
			.accelerationStructureCount = 1,
			.pAccelerationStructures = &m_TopLevelAccelerationStructure,
		};

		VkWriteDescriptorSet accelerationStructureWrite
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = &accelerationStructureInfo,
			.dstSet = descriptorSet,
			.dstBinding = 1,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
		};

		std::array<VkWriteDescriptorSet, 2> writes = { write, accelerationStructureWrite };
		vkUpdateDescriptorSets(m_Graphics->GetDevice(), static_cast<uint32>(writes.size()), writes.data(), 0, nullptr);
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

	void VkRaytracingRenderPipeline::CreateTestTriangleAccelerationStructures()
	{
		static constexpr std::array<RaytracingVertex, 3> kVertices =
		{
			RaytracingVertex{ 0.0f, 1.0f, 0.0f },
			RaytracingVertex{ 1.0f, -1.0f, 0.0f },
			RaytracingVertex{ -1.0f, -1.0f, 0.0f },
		};

		static constexpr std::array<uint32, 3> kIndices = { 0, 1, 2 };

		CreateBuffer(
			sizeof(kVertices),
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&m_TriangleVertexBuffer
		);
		UploadBuffer(&m_TriangleVertexBuffer, std::span<const byte>(reinterpret_cast<const byte*>(kVertices.data()), sizeof(kVertices)));

		CreateBuffer(
			sizeof(kIndices),
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&m_TriangleIndexBuffer
		);
		UploadBuffer(&m_TriangleIndexBuffer, std::span<const byte>(reinterpret_cast<const byte*>(kIndices.data()), sizeof(kIndices)));

		VkAccelerationStructureGeometryTrianglesDataKHR triangleData
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR,
			.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT,
			.vertexData = { .deviceAddress = GetBufferDeviceAddress(m_TriangleVertexBuffer) },
			.vertexStride = sizeof(RaytracingVertex),
			.maxVertex = static_cast<uint32>(kVertices.size() - 1),
			.indexType = VK_INDEX_TYPE_UINT32,
			.indexData = { .deviceAddress = GetBufferDeviceAddress(m_TriangleIndexBuffer) },
		};

		VkAccelerationStructureGeometryKHR triangleGeometry
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
			.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR,
			.geometry = { .triangles = triangleData },
			.flags = VK_GEOMETRY_OPAQUE_BIT_KHR,
		};

		VkAccelerationStructureBuildGeometryInfoKHR bottomLevelBuildInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
			.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
			.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
			.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
			.geometryCount = 1,
			.pGeometries = &triangleGeometry,
		};

		uint32 primitiveCount = 1;
		VkAccelerationStructureBuildSizesInfoKHR bottomLevelSizeInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR,
		};
		m_Graphics->GetAccelerationStructureBuildSizesKHRFunction()(
			m_Graphics->GetDevice(),
			VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
			&bottomLevelBuildInfo,
			&primitiveCount,
			&bottomLevelSizeInfo
		);

		CreateBuffer(
			bottomLevelSizeInfo.accelerationStructureSize,
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			&m_BottomLevelAccelerationStructureBuffer
		);

		VkAccelerationStructureCreateInfoKHR bottomLevelCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR,
			.buffer = m_BottomLevelAccelerationStructureBuffer.m_Buffer,
			.size = bottomLevelSizeInfo.accelerationStructureSize,
			.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR,
		};
		VKR(m_Graphics->GetCreateAccelerationStructureKHRFunction()(m_Graphics->GetDevice(), &bottomLevelCreateInfo, nullptr, &m_BottomLevelAccelerationStructure));

		bottomLevelBuildInfo.dstAccelerationStructure = m_BottomLevelAccelerationStructure;

		VkTransformMatrixKHR identityTransform
		{
			.matrix =
			{
				{ 1.0f, 0.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f, 0.0f },
			},
		};

		VkAccelerationStructureInstanceKHR instance = {};
		instance.transform = identityTransform;
		instance.instanceCustomIndex = 0;
		instance.mask = 0xFF;
		instance.instanceShaderBindingTableRecordOffset = 0;
		instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
		instance.accelerationStructureReference = GetAccelerationStructureDeviceAddress(m_BottomLevelAccelerationStructure);

		CreateBuffer(
			sizeof(instance),
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&m_InstanceBuffer
		);
		UploadBuffer(&m_InstanceBuffer, std::span<const byte>(reinterpret_cast<const byte*>(&instance), sizeof(instance)));

		VkAccelerationStructureGeometryInstancesDataKHR instancesData
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR,
			.arrayOfPointers = VK_FALSE,
			.data = { .deviceAddress = GetBufferDeviceAddress(m_InstanceBuffer) },
		};

		VkAccelerationStructureGeometryKHR topLevelGeometry
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
			.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR,
			.geometry = { .instances = instancesData },
		};

		VkAccelerationStructureBuildGeometryInfoKHR topLevelBuildInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR,
			.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR,
			.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
			.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR,
			.geometryCount = 1,
			.pGeometries = &topLevelGeometry,
		};

		VkAccelerationStructureBuildSizesInfoKHR topLevelSizeInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR,
		};
		m_Graphics->GetAccelerationStructureBuildSizesKHRFunction()(
			m_Graphics->GetDevice(),
			VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
			&topLevelBuildInfo,
			&primitiveCount,
			&topLevelSizeInfo
		);

		CreateBuffer(
			topLevelSizeInfo.accelerationStructureSize,
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			&m_TopLevelAccelerationStructureBuffer
		);

		VkAccelerationStructureCreateInfoKHR topLevelCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR,
			.buffer = m_TopLevelAccelerationStructureBuffer.m_Buffer,
			.size = topLevelSizeInfo.accelerationStructureSize,
			.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR,
		};
		VKR(m_Graphics->GetCreateAccelerationStructureKHRFunction()(m_Graphics->GetDevice(), &topLevelCreateInfo, nullptr, &m_TopLevelAccelerationStructure));

		topLevelBuildInfo.dstAccelerationStructure = m_TopLevelAccelerationStructure;

		VkDeviceSize scratchSize = std::max(bottomLevelSizeInfo.buildScratchSize, topLevelSizeInfo.buildScratchSize);
		CreateBuffer(
			scratchSize,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			&m_ScratchBuffer
		);

		VkDeviceAddress scratchAddress = GetBufferDeviceAddress(m_ScratchBuffer);
		bottomLevelBuildInfo.scratchData.deviceAddress = scratchAddress;
		topLevelBuildInfo.scratchData.deviceAddress = scratchAddress;

		VkAccelerationStructureBuildRangeInfoKHR bottomLevelRange
		{
			.primitiveCount = primitiveCount,
		};
		const VkAccelerationStructureBuildRangeInfoKHR* bottomLevelRanges[] = { &bottomLevelRange };

		VkAccelerationStructureBuildRangeInfoKHR topLevelRange
		{
			.primitiveCount = primitiveCount,
		};
		const VkAccelerationStructureBuildRangeInfoKHR* topLevelRanges[] = { &topLevelRange };

		auto commandBuffer = m_Graphics->GetPooledCommandBuffer();
		commandBuffer->BeginCommands();
		auto* vkCommandBuffer = static_cast<VkCommandBuffer*>(commandBuffer.Get());

		m_Graphics->GetCmdBuildAccelerationStructuresKHRFunction()(vkCommandBuffer->GetVkCommandBuffer(), 1, &bottomLevelBuildInfo, bottomLevelRanges);

		VkMemoryBarrier accelerationStructureBuildBarrier
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
			.srcAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR,
			.dstAccessMask = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR,
		};

		vkCmdPipelineBarrier(
			vkCommandBuffer->GetVkCommandBuffer(),
			VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
			VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
			0,
			1,
			&accelerationStructureBuildBarrier,
			0,
			nullptr,
			0,
			nullptr
		);

		m_Graphics->GetCmdBuildAccelerationStructuresKHRFunction()(vkCommandBuffer->GetVkCommandBuffer(), 1, &topLevelBuildInfo, topLevelRanges);

		commandBuffer->EndCommands();
		commandBuffer->WaitForCompletion(TimeSpan::FromSeconds(30));
		m_Graphics->ReleasePooledCommandBuffer(commandBuffer);
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

		m_ShaderBindingTableAddress = GetVkBufferDeviceAddress(m_Graphics, m_ShaderBindingTableBuffer);
		m_RayGenerationShaderBindingTable.deviceAddress = m_ShaderBindingTableAddress + rayGenerationOffset;
		m_MissShaderBindingTable.deviceAddress = m_ShaderBindingTableAddress + missOffset;
		if (m_HitShaderBindingTable.size > 0)
		{
			m_HitShaderBindingTable.deviceAddress = m_ShaderBindingTableAddress + hitOffset;
		}
	}

	void VkRaytracingRenderPipeline::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, BufferAllocation* outBuffer)
	{
		auto device = m_Graphics->GetDevice();

		VkBufferCreateInfo bufferInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = size,
			.usage = usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		};

		VKR(vkCreateBuffer(device, &bufferInfo, nullptr, &outBuffer->m_Buffer));

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(device, outBuffer->m_Buffer, &memoryRequirements);

		uint32 memoryTypeIndex = m_Graphics->FindMemoryType(memoryRequirements.memoryTypeBits, properties);
		if (memoryTypeIndex == UINT32_MAX)
		{
			throw InvalidOperationException(TEXT("Could not find memory for a Vulkan raytracing buffer."));
		}

		VkMemoryAllocateFlagsInfo allocateFlagsInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
			.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT,
		};

		VkMemoryAllocateInfo allocateInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT) != 0 ? &allocateFlagsInfo : nullptr,
			.allocationSize = memoryRequirements.size,
			.memoryTypeIndex = memoryTypeIndex,
		};

		VKR(vkAllocateMemory(device, &allocateInfo, nullptr, &outBuffer->m_Memory));
		VKR(vkBindBufferMemory(device, outBuffer->m_Buffer, outBuffer->m_Memory, 0));
		outBuffer->m_Size = size;
	}

	void VkRaytracingRenderPipeline::DestroyBuffer(BufferAllocation* buffer) noexcept
	{
		auto device = m_Graphics->GetDevice();
		if (buffer->m_Buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device, buffer->m_Buffer, nullptr);
			buffer->m_Buffer = VK_NULL_HANDLE;
		}
		if (buffer->m_Memory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device, buffer->m_Memory, nullptr);
			buffer->m_Memory = VK_NULL_HANDLE;
		}
		buffer->m_Size = 0;
	}

	void VkRaytracingRenderPipeline::UploadBuffer(BufferAllocation* buffer, std::span<const byte> data)
	{
		if (data.size() > buffer->m_Size)
		{
			throw InvalidOperationException(TEXT("Upload data is larger than the Vulkan raytracing buffer."));
		}

		void* mappedData = nullptr;
		VKR(vkMapMemory(m_Graphics->GetDevice(), buffer->m_Memory, 0, data.size(), 0, &mappedData));
		std::memcpy(mappedData, data.data(), data.size());
		vkUnmapMemory(m_Graphics->GetDevice(), buffer->m_Memory);
	}

	VkDeviceAddress VkRaytracingRenderPipeline::GetBufferDeviceAddress(const BufferAllocation& buffer) const
	{
		return GetVkBufferDeviceAddress(m_Graphics, buffer.m_Buffer);
	}

	VkDeviceAddress VkRaytracingRenderPipeline::GetAccelerationStructureDeviceAddress(VkAccelerationStructureKHR accelerationStructure) const
	{
		VkAccelerationStructureDeviceAddressInfoKHR addressInfo
		{
			.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR,
			.accelerationStructure = accelerationStructure,
		};

		return m_Graphics->GetAccelerationStructureDeviceAddressKHRFunction()(m_Graphics->GetDevice(), &addressInfo);
	}
}
