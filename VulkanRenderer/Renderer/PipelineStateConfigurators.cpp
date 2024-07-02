#include "PipelineStateConfigurators.h"

namespace Fox
{
	namespace Config
	{
		VertexInputConfig::VertexInputConfig()
			:
			m_VertexAttributeC(2),
			m_VertexInputAttributeDescriptions(), m_BindingDescription(),
			m_VertexInputI()
		{
			m_BindingDescription.binding = 0;
			m_BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			m_BindingDescription.stride = sizeof(Vertex);

			m_VertexInputAttributeDescriptions.resize(m_VertexAttributeC);

			m_VertexInputAttributeDescriptions[0].binding = 0;
			m_VertexInputAttributeDescriptions[0].location = 0;
			m_VertexInputAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			m_VertexInputAttributeDescriptions[0].offset = offsetof(Vertex, Position);

			m_VertexInputAttributeDescriptions[1].binding = 0;
			m_VertexInputAttributeDescriptions[1].location = 1;
			m_VertexInputAttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			m_VertexInputAttributeDescriptions[1].offset = offsetof(Vertex, Colour);

			m_VertexInputI.flags = 0;
			m_VertexInputI.pNext = nullptr;
			m_VertexInputI.pVertexAttributeDescriptions = m_VertexInputAttributeDescriptions.data();
			m_VertexInputI.pVertexBindingDescriptions = &m_BindingDescription;
			m_VertexInputI.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			m_VertexInputI.vertexAttributeDescriptionCount =
				(uint32_t)m_VertexInputAttributeDescriptions.size();
			m_VertexInputI.vertexBindingDescriptionCount = 1;
		}

		Fox::Config::InputAssemblyConfig::InputAssemblyConfig()
			:m_IAStateCreateI()
		{
			m_IAStateCreateI.flags = 0;
			m_IAStateCreateI.pNext = nullptr;
			m_IAStateCreateI.primitiveRestartEnable = false;
			m_IAStateCreateI.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			m_IAStateCreateI.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}

		DynamicStatesConfig::DynamicStatesConfig()
			:
			m_DynamicStates({VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR}),
			m_DynamicStateCreateI()
		{
			m_DynamicStateCreateI.dynamicStateCount = (uint32_t)m_DynamicStates.size();
			m_DynamicStateCreateI.flags = 0;
			m_DynamicStateCreateI.pDynamicStates = m_DynamicStates.data();
			m_DynamicStateCreateI.pNext = nullptr;
			m_DynamicStateCreateI.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		}

		RasterizerConfig::RasterizerConfig()
			:m_RasterizerStateCreateI()
		{
			m_RasterizerStateCreateI.cullMode = VK_CULL_MODE_BACK_BIT;
			m_RasterizerStateCreateI.depthBiasClamp = 0;
			m_RasterizerStateCreateI.depthBiasConstantFactor = 0;
			m_RasterizerStateCreateI.depthBiasEnable = false;
			m_RasterizerStateCreateI.depthBiasSlopeFactor = 0;
			m_RasterizerStateCreateI.depthClampEnable = false;
			m_RasterizerStateCreateI.flags = 0;
			m_RasterizerStateCreateI.frontFace = VK_FRONT_FACE_CLOCKWISE;
			m_RasterizerStateCreateI.lineWidth = 1.0f;
			m_RasterizerStateCreateI.pNext = nullptr;
			m_RasterizerStateCreateI.polygonMode = VK_POLYGON_MODE_FILL;
			m_RasterizerStateCreateI.rasterizerDiscardEnable = false;
			m_RasterizerStateCreateI.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		}

		MultisampleConfig::MultisampleConfig()
			:m_MSCreateI()
		{
			m_MSCreateI.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			m_MSCreateI.sampleShadingEnable = VK_FALSE;
			m_MSCreateI.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			m_MSCreateI.minSampleShading = 1.0f; 
			m_MSCreateI.pSampleMask = nullptr; 
			m_MSCreateI.alphaToCoverageEnable = VK_FALSE;
			m_MSCreateI.alphaToOneEnable = VK_FALSE; 
		}

		ColourBlendConfig::ColourBlendConfig()
			:m_CBCreateI(), m_CBAState()
		{
			m_CBAState.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			m_CBAState.blendEnable = VK_FALSE;

			m_CBCreateI.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			m_CBCreateI.logicOpEnable = VK_FALSE;
			m_CBCreateI.logicOp = VK_LOGIC_OP_COPY; 
			m_CBCreateI.attachmentCount = 1;
			m_CBCreateI.pAttachments = &m_CBAState;
			m_CBCreateI.blendConstants[0] = 0.0f; 
			m_CBCreateI.blendConstants[1] = 0.0f; 
			m_CBCreateI.blendConstants[2] = 0.0f; 
			m_CBCreateI.blendConstants[3] = 0.0f; 
		}

		ViewportConfig::ViewportConfig()
			:
			m_Viewport(),
			m_Scissors(),
			m_VPStateCreateI()
		{
			m_VPStateCreateI.flags = 0;
			m_VPStateCreateI.pNext = nullptr;
			m_VPStateCreateI.pScissors = nullptr;
			m_VPStateCreateI.pViewports = nullptr;
			m_VPStateCreateI.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			m_VPStateCreateI.viewportCount = 1;
			m_VPStateCreateI.scissorCount = 1;
		}

		void ViewportConfig::SetViewportAndScissors(VkExtent2D& Extent) noexcept
		{
			m_Viewport.x = 0.0f;
			m_Viewport.y = 0.0f;
			m_Viewport.height = (float)Extent.height;
			m_Viewport.width = (float)Extent.width;
			m_Viewport.maxDepth = 1.0f;
			m_Viewport.minDepth = 0.0f;

			m_Scissors.offset = { 0,0 };
			m_Scissors.extent = Extent;
		}
}
}