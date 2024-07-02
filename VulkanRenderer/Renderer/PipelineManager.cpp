#include "PipelineManager.h"

namespace Fox
{
	namespace Resource
	{
		RenderPass::RenderPass(VkDevice* pValidDevice, VkSurfaceFormatKHR& Format)
			:
			CResource(pValidDevice),
			m_RenderPass(VK_NULL_HANDLE)
		{
			VkAttachmentDescription AttachmentDesc = {};
			AttachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			AttachmentDesc.flags = 0;
			AttachmentDesc.format = Format.format;
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

			Debug::Result = vkCreateRenderPass(*pDevice, &RenderPassI, nullptr, &m_RenderPass);
			CHECK(Debug::Result, "Render Pass Creation Failed!");
		}

		void RenderPass::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(m_RenderPass);
			vkDestroyRenderPass(*pDevice, m_RenderPass, nullptr);
		}
	}

	namespace vk
	{
		PipelineManager::PipelineManager(DeviceManager* pDeviceManager, 
			VkSurfaceFormatKHR& SurfaceFormat)
			:
			DLManager(pDeviceManager->GetDeviceH()),

			m_IAConfig(),
			m_VIConfig(),
			m_DynamicStatesConfig(),
			m_RasterizerStateConfig(),
			m_MSConfig(),
			m_CBConfig(),
			m_VPConfig(),

			m_ShaderManager(pDeviceManager),
			m_RenderPass(pDeviceManager->GetDeviceH(),SurfaceFormat),

			m_ShaderStages({ m_ShaderManager.GetVertexShaderH().GetShaderStageCreateI(),
				m_ShaderManager.GetPixelShaderH().GetShaderStageCreateI() }),

			m_PipelineLayout(VK_NULL_HANDLE), 
			m_GraphicsPipeline(VK_NULL_HANDLE)
		{

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

			VkGraphicsPipelineCreateInfo PipelineI = {};
			PipelineI.pInputAssemblyState = m_IAConfig.GetStateCreateInfo();
			PipelineI.pVertexInputState = m_VIConfig.GetStateCreateInfo();
			PipelineI.pDynamicState = m_DynamicStatesConfig.GetStateCreateInfo();
			PipelineI.pRasterizationState = m_RasterizerStateConfig.GetStateCreateInfo();
			PipelineI.pMultisampleState = m_MSConfig.GetStateCreateInfo();
			PipelineI.pColorBlendState = m_CBConfig.GetStateCreateInfo();
			PipelineI.pViewportState = m_VPConfig.GetStateCreateInfo();
			PipelineI.renderPass = *m_RenderPass.GetRenderPassH();
			PipelineI.basePipelineHandle = nullptr;
			PipelineI.basePipelineIndex = -1;
			PipelineI.flags = 0;
			PipelineI.layout = m_PipelineLayout;
			PipelineI.pDepthStencilState = nullptr;
			PipelineI.pNext = nullptr;
			PipelineI.pStages = m_ShaderStages.data();
			PipelineI.pTessellationState = nullptr;
			PipelineI.stageCount = 2;
			PipelineI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			PipelineI.subpass = 0;

			Debug::Result = vkCreateGraphicsPipelines(*pDevice, nullptr, 1, &PipelineI,
				nullptr, &m_GraphicsPipeline);
			CHECK(Debug::Result, "Graphics Pipeline Creation Failed!");
		}

		void PipelineManager::ReconstructGraphicsPipeline(VkSurfaceFormatKHR& SurfaceFormat)
		{
			m_RenderPass.DestroyResource();
			vkDestroyPipeline(*pDevice, m_GraphicsPipeline, nullptr);
			m_GraphicsPipeline = VK_NULL_HANDLE;

			m_RenderPass = Resource::RenderPass(pDevice, SurfaceFormat);

			VkGraphicsPipelineCreateInfo PipelineI = {};
			PipelineI.pInputAssemblyState		= m_IAConfig.GetStateCreateInfo();
			PipelineI.pVertexInputState			= m_VIConfig.GetStateCreateInfo();
			PipelineI.pDynamicState				= m_DynamicStatesConfig.GetStateCreateInfo();
			PipelineI.pRasterizationState		= m_RasterizerStateConfig.GetStateCreateInfo();
			PipelineI.pMultisampleState			= m_MSConfig.GetStateCreateInfo();
			PipelineI.pColorBlendState			= m_CBConfig.GetStateCreateInfo();
			PipelineI.pViewportState			= m_VPConfig.GetStateCreateInfo();
			PipelineI.renderPass				= *m_RenderPass.GetRenderPassH();
			PipelineI.basePipelineHandle		= nullptr;
			PipelineI.basePipelineIndex			= -1;
			PipelineI.flags						= 0;
			PipelineI.layout					= m_PipelineLayout;
			PipelineI.pDepthStencilState		= nullptr;
			PipelineI.pNext						= nullptr;
			PipelineI.pStages					= m_ShaderStages.data();
			PipelineI.pTessellationState		= nullptr;
			PipelineI.stageCount				= 2;
			PipelineI.sType						= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			PipelineI.subpass					= 0;

			Debug::Result = vkCreateGraphicsPipelines(*pDevice, nullptr, 1, &PipelineI,
				nullptr, &m_GraphicsPipeline);
			CHECK(Debug::Result, "Graphics Pipeline Creation Failed!");
		}

		void PipelineManager::DestroyResources() noexcept
		{
			m_ShaderManager.DestroyResources();
			m_RenderPass.DestroyResource();

			//vkDestroyDescriptorSetLayout(*pDevice, m_UniformLayout, nullptr);

			vkDestroyPipelineLayout(*pDevice, m_PipelineLayout, nullptr);

			vkDestroyPipeline(*pDevice, m_GraphicsPipeline, nullptr);
		}
	}
	
}

