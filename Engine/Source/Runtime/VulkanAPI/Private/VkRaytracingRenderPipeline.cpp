// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkRaytracingRenderPipeline.h"
#include "VkGraphics.h"
#include "VkShader.h"
#include "Rendering/ShaderType.h"

namespace Ayla
{
	namespace
	{
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
	}

	VkRaytracingRenderPipeline::~VkRaytracingRenderPipeline() noexcept
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
		if (m_DescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
		}
	}

	void VkRaytracingRenderPipeline::SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset)
	{
	}
}
