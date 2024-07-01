
#pragma once

#include "Buffer.h"

namespace Fox
{
	namespace Resource
	{
		class TriangleMesh : public Mesh
		{
		public:
			TriangleMesh(const TriangleMesh& Other) = delete;
			TriangleMesh(TriangleMesh&& Other) = default;

			TriangleMesh(VkDevice* pDevice, VmaAllocator* pAllocator, CommandBuffer* pTransferCmdBuffer,
				VkQueue* pQueue);
			~TriangleMesh() = default;

			virtual void Draw(void* pCommandBuffer) noexcept override;
			virtual void DestroyResources() noexcept override;

		private:
			std::vector<Vertex>		m_Vertices;
			Buffer					m_VertexBuffer;
		};
	}
}

