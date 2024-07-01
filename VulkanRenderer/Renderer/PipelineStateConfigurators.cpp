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
	}
}