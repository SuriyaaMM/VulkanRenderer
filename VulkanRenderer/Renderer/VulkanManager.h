
#pragma once

#include "InstanceManager.h"
#include "FramebufferManager.h"
#include "CommandBufferManager.h"
#include "SyncManager.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Frame.h"

namespace Fox
{
	namespace vk
	{
		class VulkanManager
		{
		public:
			VulkanManager(const VulkanManager& Source) = delete;
			VulkanManager(VulkanManager&& Source) = delete;

			VulkanManager();
			~VulkanManager();

			void DrawFrame() noexcept;
			void RecreateFramebuffers() noexcept;

			WindowManager& GetWindowManager() noexcept { return m_WindowManager; }
			DeviceManager& GetDeviceHManager() noexcept { return m_DeviceManager; }

		private:

			void RecordDrawCommand(VkFramebuffer* pFramebuffer,
				Resource::CommandBuffer* pCommandBuffer) noexcept;

			void UpdateUniform(uint32_t ImageIndex) noexcept;

		private:
			std::vector<const char*> m_RInstanceExtensions;
			std::vector<const char*> m_RDeviceExtensions;
			std::vector<const char*> m_RInstanceLayers;
			std::vector<const char*> m_RDeviceLayers;

			uint32_t m_BackbufferIndex;

			InstanceManager			m_InstanceManager;
			WindowManager			m_WindowManager;
			DeviceManager			m_DeviceManager;
			MemoryManager			m_MemoryManager;
			
			std::vector<Resource::Frame> m_Frames;

			SwapchainManager		m_SwapchainManager;
			PipelineManager			m_PipelineManager;
			std::unique_ptr<ImageManager>				m_pImageManager;
			std::unique_ptr<FramebufferManager>			m_pFramebufferManager;
			CommandBufferManager	m_CommandBufferManager;
			SyncManager				m_SyncManager;
			ResourceManager			m_ResourceManager;

			Resource::TriangleMesh	m_TriangleMesh;

			VkDescriptorPool					m_DescriptorPool;
			std::vector<VkDescriptorSetLayout>	m_UniformLayouts;
			std::vector<VkDescriptorSet>		m_UniformDescriptors;
		};
	}
}

