
#pragma once

#include "ImageManager.h"
#include "PipelineManager.h"

namespace Fox
{
	namespace Resource
	{
		class Framebuffer : public MResource
		{
		public:
			Framebuffer(const Framebuffer& Other) = delete;
			Framebuffer(Framebuffer&& Other) = default;

			Framebuffer(VkDevice* pValidDevice, VkExtent2D& Extent, VkRenderPass* pRenderPass, 
				VkImageView* pArrayImageViews, uint32_t&& AttachedImageViewsCount);

			~Framebuffer() = default;

			virtual void DestroyResource() noexcept override;

			VkFramebuffer* GetFramebufferH() noexcept { return &m_Framebuffer; }

		private:
			VkFramebuffer m_Framebuffer;
		};
	}

	namespace vk
	{
		class FramebufferManager : public Manager
		{
		public:
			FramebufferManager(const FramebufferManager& Other) = delete;
			FramebufferManager(FramebufferManager&& Other) = delete;

			FramebufferManager(DeviceManager* pDeviceManager, SwapchainManager* pSwapchainManager,
				PipelineManager* pPipelineManager, ImageManager* pImageManager);
			~FramebufferManager() = default;

			void RecreateFramebuffers(DeviceManager* pDeviceManager, SwapchainManager* pSwapchainManager,
				PipelineManager* pPipelineManager, ImageManager* pImageManager) noexcept;

			virtual void DestroyResources() noexcept override;

			std::vector<Resource::Framebuffer>& GetFramebufferVH() noexcept { return m_Framebuffers; }
		
		private:
			std::vector<Resource::Framebuffer> m_Framebuffers;
		};
	}
}

