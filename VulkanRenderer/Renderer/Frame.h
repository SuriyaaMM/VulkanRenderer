
#pragma once

#include "FramebufferManager.h"
#include "CommandPoolManager.h"
#include "PipelineManager.h"
#include "SyncResources.h"

namespace Fox
{
	namespace Resource
	{
		class Frame : public MResource
		{
		public:
			Frame(const Frame& Other) = delete;
			Frame(Frame&& Other) = default;

			Frame(vk::DeviceManager* pDeviceManager, vk::WindowManager* pWindowManager,
				vk::CommandPoolManager* pCommandPoolManager);
			~Frame() = default;

			void PresentFrame(Swapchain* pSwapchain, VkQueue* pGraphicsQueue, uint32_t ImageIndex) noexcept;

			VkFence*	 GetFenceH()				   noexcept { return m_AsyncFence.GetFenceH(); }
			VkSemaphore& GetImageAvailableSemaphoreR() noexcept { return *m_ImageAvailable.GetSemaphoreH(); }
			VkSemaphore& GetRenderFinishedSemaphoreR() noexcept { return *m_RenderFinished.GetSemaphoreH(); }
			CommandBuffer* GetCommandBufferH() noexcept { return &m_CommandBuffer; }

			virtual void DestroyResource() noexcept;
		
		private:
			Resource::CommandBuffer m_CommandBuffer;
			Resource::Semaphore		m_RenderFinished;
			Resource::Semaphore		m_ImageAvailable;
			Resource::Fence			m_AsyncFence;
		};
	}
}

