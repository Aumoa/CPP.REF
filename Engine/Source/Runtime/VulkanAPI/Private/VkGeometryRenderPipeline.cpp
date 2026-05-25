// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkGeometryRenderPipeline.h"
#include "VkShader.h"
#include "VkGraphics.h"
#include "VkCommandBuffer.h"
#include "VkBuffer.h"
#include "VkSwapchainRenderTexture.h"
#include "Rendering/VertexFactory.h"
#include "Rendering/ShaderType.h"

namespace Ayla
{
	VkGeometryRenderPipeline::VkGeometryRenderPipeline(VkGraphics* graphics, SharedPtr<Shader> shader)
		: m_Graphics(graphics)
	{
		auto device = graphics->GetDevice();

		// Create descriptor set layout (set 0, binding 0: uniform buffer for camera)
		VkDescriptorSetLayoutBinding uboBinding
		{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		};

		VkDescriptorSetLayoutCreateInfo layoutInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = 1,
			.pBindings = &uboBinding,
		};

		VKR(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayout));

		// Create pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 1,
			.pSetLayouts = &m_DescriptorSetLayout,
		};

		VKR(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout));

		// Create a compatible render pass for pipeline creation
		VkAttachmentDescription attachments[2] =
		{
			// Color attachment
			{
				.format = VkSwapchainRenderTexture::GetColorFormat(),
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			},
			// Depth attachment
			{
				.format = VkSwapchainRenderTexture::GetDepthFormat(),
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			}
		};

		VkAttachmentReference colorRef{ .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		VkAttachmentReference depthRef{ .attachment = 1, .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

		VkSubpassDescription subpass
		{
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.colorAttachmentCount = 1,
			.pColorAttachments = &colorRef,
			.pDepthStencilAttachment = &depthRef,
		};

		VkSubpassDependency dependency
		{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		};

		VkRenderPassCreateInfo renderPassInfo
		{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.attachmentCount = 2,
			.pAttachments = attachments,
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &dependency,
		};

		VKR(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_RenderPass));

		// Create shader modules
		VkShaderModule vertModule = VK_NULL_HANDLE;
		VkShaderModule fragModule = VK_NULL_HANDLE;

		if (shader->Has(ShaderType::Vertex))
		{
			const auto& vs = shader->GetBytecode(ShaderType::Vertex);
			VkShaderModuleCreateInfo moduleInfo
			{
				.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				.codeSize = vs.Bytecode.size(),
				.pCode = reinterpret_cast<const uint32_t*>(vs.Bytecode.data()),
			};
			VKR(vkCreateShaderModule(device, &moduleInfo, nullptr, &vertModule));
		}

		if (shader->Has(ShaderType::Pixel))
		{
			const auto& ps = shader->GetBytecode(ShaderType::Pixel);
			VkShaderModuleCreateInfo moduleInfo
			{
				.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				.codeSize = ps.Bytecode.size(),
				.pCode = reinterpret_cast<const uint32_t*>(ps.Bytecode.data()),
			};
			VKR(vkCreateShaderModule(device, &moduleInfo, nullptr, &fragModule));
		}

		VkPipelineShaderStageCreateInfo shaderStages[2] =
		{
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_VERTEX_BIT,
				.module = vertModule,
				.pName = "main",
			},
			{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
				.module = fragModule,
				.pName = "main",
			}
		};

		// Vertex input state from vertex factory
		std::vector<VkVertexInputAttributeDescription> vertexAttributes;
		VkVertexInputBindingDescription vertexBinding{};
		uint32_t totalStride = 0;

		if (auto* vf = shader->GetVertexFactory())
		{
			const auto& elements = vf->GetInputElements();
			vertexAttributes.reserve(elements.size());

			for (uint32_t i = 0; i < (uint32_t)elements.size(); ++i)
			{
				VkVertexInputAttributeDescription attr
				{
					.location = i,
					.binding = (uint32_t)elements[i].InputSlot,
					.format = ToVkFormat(elements[i].Format),
					.offset = (uint32_t)elements[i].AlignedByteOffset,
				};
				vertexAttributes.emplace_back(attr);

				// Track max offset + format size for stride calculation
				uint32_t formatSize = 0;
				switch (elements[i].Format)
				{
				case GraphicsFormat::R32G32B32A32_Float: formatSize = 16; break;
				case GraphicsFormat::R32G32B32_Float: formatSize = 12; break;
				case GraphicsFormat::R32G32_Float: formatSize = 8; break;
				case GraphicsFormat::R32_Float: formatSize = 4; break;
				default: formatSize = 4; break;
				}
				uint32_t end = (uint32_t)elements[i].AlignedByteOffset + formatSize;
				if (end > totalStride)
				{
					totalStride = end;
				}
			}

			vertexBinding =
			{
				.binding = 0,
				.stride = totalStride,
				.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
			};
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount = (totalStride > 0) ? 1u : 0u,
			.pVertexBindingDescriptions = &vertexBinding,
			.vertexAttributeDescriptionCount = (uint32_t)vertexAttributes.size(),
			.pVertexAttributeDescriptions = vertexAttributes.data(),
		};

		VkPipelineInputAssemblyStateCreateInfo inputAssembly
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE,
		};

		VkPipelineViewportStateCreateInfo viewportState
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = 1,
			.scissorCount = 1,
		};

		VkPipelineRasterizationStateCreateInfo rasterizer
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.lineWidth = 1.0f,
		};

		VkPipelineMultisampleStateCreateInfo multisampling
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
		};

		VkPipelineDepthStencilStateCreateInfo depthStencil
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.depthTestEnable = VK_TRUE,
			.depthWriteEnable = VK_TRUE,
			.depthCompareOp = VK_COMPARE_OP_LESS,
			.depthBoundsTestEnable = VK_FALSE,
			.stencilTestEnable = VK_FALSE,
		};

		VkPipelineColorBlendAttachmentState colorBlendAttachment
		{
			.blendEnable = VK_FALSE,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		};

		VkPipelineColorBlendStateCreateInfo colorBlending
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable = VK_FALSE,
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachment,
		};

		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = 2,
			.pDynamicStates = dynamicStates,
		};

		VkGraphicsPipelineCreateInfo pipelineInfo
		{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.stageCount = 2,
			.pStages = shaderStages,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = &inputAssembly,
			.pViewportState = &viewportState,
			.pRasterizationState = &rasterizer,
			.pMultisampleState = &multisampling,
			.pDepthStencilState = &depthStencil,
			.pColorBlendState = &colorBlending,
			.pDynamicState = &dynamicState,
			.layout = m_PipelineLayout,
			.renderPass = m_RenderPass,
			.subpass = 0,
		};

		VKR(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline));

		// Cleanup shader modules
		if (vertModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(device, vertModule, nullptr);
		}
		if (fragModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(device, fragModule, nullptr);
		}

		// Create descriptor pool and sets
		VkDescriptorPoolSize poolSize
		{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = (uint32_t)Graphics::kMaxFramesInFlight,
		};

		VkDescriptorPoolCreateInfo poolInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.maxSets = (uint32_t)Graphics::kMaxFramesInFlight,
			.poolSizeCount = 1,
			.pPoolSizes = &poolSize,
		};

		VKR(vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_DescriptorPool));

		std::vector<VkDescriptorSetLayout> layouts(Graphics::kMaxFramesInFlight, m_DescriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo
		{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = m_DescriptorPool,
			.descriptorSetCount = (uint32_t)Graphics::kMaxFramesInFlight,
			.pSetLayouts = layouts.data(),
		};

		m_DescriptorSets.resize(Graphics::kMaxFramesInFlight);
		VKR(vkAllocateDescriptorSets(device, &allocInfo, m_DescriptorSets.data()));
	}

	VkGeometryRenderPipeline::~VkGeometryRenderPipeline() noexcept
	{
		auto device = m_Graphics->GetDevice();

		if (m_DescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(device, m_DescriptorPool, nullptr);
		}
		if (m_Pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(device, m_Pipeline, nullptr);
		}
		if (m_RenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(device, m_RenderPass, nullptr);
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

	void VkGeometryRenderPipeline::SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset)
	{
		auto* vkCmd = static_cast<VkCommandBuffer*>(cmd);
		auto* vkBuffer = static_cast<VkBuffer*>(buffer);
		auto frameIndex = m_Graphics->GetFrameIndex();

		VkDescriptorBufferInfo bufferInfo
		{
			.buffer = vkBuffer->GetVkBuffer(),
			.offset = offset,
			.range = VK_WHOLE_SIZE,
		};

		VkWriteDescriptorSet write
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_DescriptorSets[frameIndex],
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pBufferInfo = &bufferInfo,
		};

		vkUpdateDescriptorSets(m_Graphics->GetDevice(), 1, &write, 0, nullptr);
		vkCmdBindDescriptorSets(
			vkCmd->GetVkCommandBuffer(),
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_PipelineLayout,
			0, 1, &m_DescriptorSets[frameIndex],
			0, nullptr
		);
	}
}
