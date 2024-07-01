#include "FramebufferManager.h"

namespace Fox
{
	namespace Resource
	{
		Framebuffer::Framebuffer(VkDevice* pValidDevice, VkExtent2D& Extent, VkRenderPass* RenderPass, 
			VkImageView* pArrayImageViews, uint32_t&& AttachedImageViewsCount)
			:
			MResource(pValidDevice)
		{
			VkFramebufferCreateInfo FramebufferI = {};
			FramebufferI.attachmentCount = 1;
			FramebufferI.flags = 0;
			FramebufferI.height = Extent.height;
			FramebufferI.layers = 1;
			FramebufferI.pAttachments = pArrayImageViews;
			FramebufferI.pNext = nullptr;
			FramebufferI.renderPass = *RenderPass;
			FramebufferI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			FramebufferI.width = Extent.width;

			Debug::Result = vkCreateFramebuffer(*pDevice, &FramebufferI, nullptr, &m_Framebuffer);
			CHECK(Debug::Result, "Framebuffer Creation Failed!");
		}

		void Framebuffer::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(pDevice);
			VALIDATE_HANDLE(m_Framebuffer);
			vkDestroyFramebuffer(*pDevice, m_Framebuffer, nullptr);
		}
	}

	namespace vk
	{
		FramebufferManager::FramebufferManager(DeviceManager* pDeviceManager, 
			SwapchainManager* pSwapchainManager, PipelineManager* pPipelineManager, 
			ImageManager* pImageManager)
			:m_Framebuffers()
		{
			for (std::size_t i = 0; i < pImageManager->GetImageViewsV().size(); ++i)
			{
				VkImageView Attachment[1] =
				{
					pImageManager->GetImageViewsV()[i]
				};

				m_Framebuffers.emplace_back(std::move(
					Resource::Framebuffer(pDeviceManager->GetDeviceH(), 
						pSwapchainManager->GetSwapchainH()->GetExtent(), 
						pPipelineManager->GetRenderPassH(), Attachment, 1)));
			}
		}

		void FramebufferManager::RecreateFramebuffers(DeviceManager* pDeviceManager, 
			SwapchainManager* pSwapchainManager, PipelineManager* pPipelineManager, 
			ImageManager* pImageManager) noexcept
		{
			DestroyResources();

			for (std::size_t i = 0; i < pImageManager->GetImageViewsV().size(); ++i)
			{
				VkImageView Attachment[1] =
				{
					pImageManager->GetImageViewsV()[i]
				};

				m_Framebuffers.emplace_back(std::move(
					Resource::Framebuffer(pDeviceManager->GetDeviceH(), 
						pSwapchainManager->GetSwapchainH()->GetExtent(),
						pPipelineManager->GetRenderPassH(), Attachment, 1)));
			}
		}

		void FramebufferManager::DestroyResources() noexcept
		{
			for (std::size_t i = 0; i < m_Framebuffers.size(); ++i)
			{
				m_Framebuffers[i].DestroyResource();
			}
		}
	}
}


