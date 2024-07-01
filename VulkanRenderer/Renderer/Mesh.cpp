#include "Mesh.h"

namespace Fox
{
	namespace Resource
	{
		TriangleMesh::TriangleMesh(VkDevice* pDevice, VmaAllocator* pAllocator, 
			CommandBuffer* pTransferCmdBuffer, VkQueue* pQueue)
			:
			m_Vertices(),
			m_VertexBuffer(pAllocator)
		{
			m_Vertices = {
				{{ 0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
				{{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
				{{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
				};

			m_VertexBuffer.CraftBuffer(Utility::SizeInBytes(m_Vertices),
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 0, VMA_MEMORY_USAGE_CPU_TO_GPU);
			
			void* pData = nullptr;

			vmaMapMemory(*pAllocator, *m_VertexBuffer.GetAllocationMetaData(), &pData);

			memcpy(pData, m_Vertices.data(), Utility::SizeInBytes(m_Vertices));

			vmaUnmapMemory(*pAllocator, *m_VertexBuffer.GetAllocationMetaData());
		}

		void TriangleMesh::Draw(void* pCommandBuffer) noexcept
		{
			VkDeviceSize Offsets[] = { 0 };

			vkCmdBindVertexBuffers(*reinterpret_cast<CommandBuffer*>(pCommandBuffer)->GetCommandBufferH(),
				0, 1, m_VertexBuffer.GetBufferH(), Offsets);

			vkCmdDraw(*reinterpret_cast<CommandBuffer*>(pCommandBuffer)->GetCommandBufferH(),
				(uint32_t)m_Vertices.size(), 1, 0, 0);
		}

		void TriangleMesh::DestroyResources() noexcept
		{
			m_VertexBuffer.DestroyResource();
		}
	}
}