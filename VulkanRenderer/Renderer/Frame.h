
#pragma once

#include "ImageManager.h"
#include "SyncManager.h"
#include "FramebufferManager.h"
#include "CommandBufferManager.h"
#include "PipelineManager.h"

namespace Fox
{
	namespace Resource
	{
		class Frame : public MResource
		{
		public:
			Frame(const Frame& Other) = delete;
			Frame(Frame&& Other) = default;

			Frame(vk::DeviceManager* pDeviceManager, vk::WindowManager* pWindowManager);
			~Frame() = default;

			void PresentFrame(Swapchain* pSwapchain, Framebuffer* pFramebuffer,
				CommandBuffer* pGraphicsCmdBuffer, VkQueue* pGraphicsQueue);

			virtual void DestroyResource() noexcept;
		
		private:
			Resource::Semaphore		m_RenderFinished;
			Resource::Semaphore		m_ImageAvailable;
			Resource::Fence			m_AsyncFence;
		};
	}
}

