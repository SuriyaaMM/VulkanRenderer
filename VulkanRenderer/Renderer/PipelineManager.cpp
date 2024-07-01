#include "PipelineManager.h"

namespace Fox
{
	namespace vk
	{
		PipelineManager::PipelineManager(DeviceManager* pDeviceManager, 
			VkSurfaceFormatKHR& SurfaceFormat)
			:
			DLManager(pDeviceManager->GetDeviceH()),
			m_IAConfig(),
			m_VIConfig(),

			m_ShaderManager(pDeviceManager),
			m_PipelineLayout(VK_NULL_HANDLE), m_GraphicsPipeline(VK_NULL_HANDLE),
			m_Viewport(), m_Scissors()
		{
			VkPipelineShaderStageCreateInfo ShaderStages[2] =
			{
				m_ShaderManager.GetVertexShaderH().GetShaderStageCreateI(),
				m_ShaderManager.GetPixelShaderH().GetShaderStageCreateI()
			};

			m_DynamicStates = {
				VK_DYNAMIC_STATE_SCISSOR,
				VK_DYNAMIC_STATE_VIEWPORT
			};

			VkPipelineDynamicStateCreateInfo DynamicStateI = {};
			DynamicStateI.dynamicStateCount = (uint32_t)m_DynamicStates.size();
			DynamicStateI.flags = 0;
			DynamicStateI.pDynamicStates = m_DynamicStates.data();
			DynamicStateI.pNext = nullptr;
			DynamicStateI.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

			VkPipelineRasterizationStateCreateInfo RasterizerI = {};
			RasterizerI.cullMode = VK_CULL_MODE_BACK_BIT;
			RasterizerI.depthBiasClamp = 0;
			RasterizerI.depthBiasConstantFactor = 0;
			RasterizerI.depthBiasEnable = false;
			RasterizerI.depthBiasSlopeFactor = 0;
			RasterizerI.depthClampEnable = false;
			RasterizerI.flags = 0;
			RasterizerI.frontFace = VK_FRONT_FACE_CLOCKWISE;
			RasterizerI.lineWidth = 1.0f;
			RasterizerI.pNext = nullptr;
			RasterizerI.polygonMode = VK_POLYGON_MODE_FILL;
			RasterizerI.rasterizerDiscardEnable = false;
			RasterizerI.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

			VkPipelineMultisampleStateCreateInfo MultisamplingI = {};
			MultisamplingI.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			MultisamplingI.sampleShadingEnable = VK_FALSE;
			MultisamplingI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			MultisamplingI.minSampleShading = 1.0f; // Optional
			MultisamplingI.pSampleMask = nullptr; // Optional
			MultisamplingI.alphaToCoverageEnable = VK_FALSE; // Optional
			MultisamplingI.alphaToOneEnable = VK_FALSE; // Optional

			VkPipelineColorBlendAttachmentState ColourBlendAttachmentI{};
			ColourBlendAttachmentI.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			ColourBlendAttachmentI.blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo ColourBlendI{};
			ColourBlendI.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			ColourBlendI.logicOpEnable = VK_FALSE;
			ColourBlendI.logicOp = VK_LOGIC_OP_COPY; // Optional
			ColourBlendI.attachmentCount = 1;
			ColourBlendI.pAttachments = &ColourBlendAttachmentI;
			ColourBlendI.blendConstants[0] = 0.0f; // Optional
			ColourBlendI.blendConstants[1] = 0.0f; // Optional
			ColourBlendI.blendConstants[2] = 0.0f; // Optional
			ColourBlendI.blendConstants[3] = 0.0f; // Optional

			/*
			VkDescriptorSetLayoutBinding UniformBinding = {};
			UniformBinding.binding = 0;
			UniformBinding.descriptorCount = 1;
			UniformBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			UniformBinding.pImmutableSamplers = nullptr;
			UniformBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

			VkDescriptorSetLayoutCreateInfo DescriptorLayoutI = {};
			DescriptorLayoutI.bindingCount = 1;
			DescriptorLayoutI.flags = 0;
			DescriptorLayoutI.pBindings = &UniformBinding;
			DescriptorLayoutI.pNext = nullptr;
			DescriptorLayoutI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

			Debug::Result = vkCreateDescriptorSetLayout(*pDevice,
				&DescriptorLayoutI, nullptr, &m_UniformLayout);
			CHECK(Debug::Result, "Uniform Layout Creation Failed!");
			*/

			VkPipelineLayoutCreateInfo PipelineLayoutI = {};
			PipelineLayoutI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			PipelineLayoutI.setLayoutCount = 0;
			PipelineLayoutI.pSetLayouts = nullptr;
			PipelineLayoutI.pushConstantRangeCount = 0; // Optional
			PipelineLayoutI.pPushConstantRanges = nullptr; // Optional

			Debug::Result = vkCreatePipelineLayout(*pDevice, &PipelineLayoutI, nullptr,
				&m_PipelineLayout);
			CHECK(Debug::Result, "Vulkan Pipeline Creation Failed!");

			VkAttachmentDescription AttachmentDesc = {};
			AttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			AttachmentDesc.flags = 0;
			AttachmentDesc.format = SurfaceFormat.format;
			AttachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			AttachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			AttachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
			AttachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			AttachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			AttachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

			VkAttachmentReference AttachmentRef = {};
			AttachmentRef.attachment = 0;
			AttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription SubpassDesc = {};
			SubpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			SubpassDesc.colorAttachmentCount = 1;
			SubpassDesc.pColorAttachments = &AttachmentRef;

			VkSubpassDependency SubpassDependency = {};
			SubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			SubpassDependency.dstSubpass = 0;
			SubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			SubpassDependency.srcAccessMask = 0;
			SubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			SubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo RenderPassI = {};
			RenderPassI.attachmentCount = 1;
			RenderPassI.dependencyCount = 1;
			RenderPassI.flags = 0;
			RenderPassI.pAttachments = &AttachmentDesc;
			RenderPassI.pDependencies = &SubpassDependency;
			RenderPassI.pNext = nullptr;
			RenderPassI.pSubpasses = &SubpassDesc;
			RenderPassI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			RenderPassI.subpassCount = 1;

			Debug::Result = vkCreateRenderPass(*pDevice, &RenderPassI, nullptr,
				&m_RenderPass);
			CHECK(Debug::Result, "Render Pass Creation Failed!");

			VkPipelineViewportStateCreateInfo ViewportStateI = {};
			ViewportStateI.flags = 0;
			ViewportStateI.pNext = nullptr;
			ViewportStateI.pScissors = nullptr;
			ViewportStateI.pViewports = nullptr;
			ViewportStateI.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			ViewportStateI.viewportCount = 1;
			ViewportStateI.scissorCount = 1;

			VkGraphicsPipelineCreateInfo PipelineI = {};
			PipelineI.basePipelineHandle = nullptr;
			PipelineI.basePipelineIndex = -1;
			PipelineI.flags = 0;
			PipelineI.layout = m_PipelineLayout;
			PipelineI.pColorBlendState = &ColourBlendI;
			PipelineI.pDepthStencilState = nullptr;
			PipelineI.pDynamicState = &DynamicStateI;
			PipelineI.pInputAssemblyState = m_IAConfig.GetStateCreateInfo();
			PipelineI.pMultisampleState = &MultisamplingI;
			PipelineI.pNext = nullptr;
			PipelineI.pRasterizationState = &RasterizerI;
			PipelineI.pStages = ShaderStages;
			PipelineI.pTessellationState = nullptr;
			PipelineI.pVertexInputState = m_VIConfig.GetStateCreateInfo();
			PipelineI.pViewportState = &ViewportStateI;
			PipelineI.renderPass = m_RenderPass;
			PipelineI.stageCount = 2;
			PipelineI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			PipelineI.subpass = 0;

			Debug::Result = vkCreateGraphicsPipelines(*pDevice, nullptr, 1, &PipelineI,
				nullptr, &m_GraphicsPipeline);
			CHECK(Debug::Result, "Graphics Pipeline Creation Failed!");
		}

		void PipelineManager::SetViewportAndScissor(VkExtent2D& Extent) noexcept
		{
			m_Viewport.x = 0.0f;
			m_Viewport.y = 0.0f;
			m_Viewport.height =	(float)Extent.height;
			m_Viewport.width =	(float)Extent.width;
			m_Viewport.maxDepth = 1.0f;
			m_Viewport.minDepth = 0.0f;

			m_Scissors.offset = { 0,0 };
			m_Scissors.extent =	Extent;
		}

		void PipelineManager::DestroyResources() noexcept
		{
			m_ShaderManager.DestroyResources();

			//vkDestroyDescriptorSetLayout(*pDevice, m_UniformLayout, nullptr);

			vkDestroyPipelineLayout(*pDevice, m_PipelineLayout, nullptr);

			vkDestroyRenderPass(*pDevice, m_RenderPass, nullptr);

			vkDestroyPipeline(*pDevice, m_GraphicsPipeline, nullptr);
		}
	}
	
}