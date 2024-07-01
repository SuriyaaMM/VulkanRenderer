#include "ImageManager.h"

namespace Fox
{
	namespace vk
	{
		ImageManager::ImageManager(DeviceManager* pDeviceManager, SwapchainManager* pSwapchainManager)
			:
			DLManager(pDeviceManager->GetDeviceH())
		{
			vkGetSwapchainImagesKHR(*pDevice, *pSwapchainManager->GetSwapchainH()->GetSwapchainH(),
				&pSwapchainManager->GetSwapchainH()->GetImageCount(), nullptr);

			m_Images.resize(pSwapchainManager->GetSwapchainH()->GetImageCount());

			vkGetSwapchainImagesKHR(*pDevice, *pSwapchainManager->GetSwapchainH()->GetSwapchainH(),
				&pSwapchainManager->GetSwapchainH()->GetImageCount(), m_Images.data());

			m_ImageViews.resize(m_Images.size());

			for (std::size_t i = 0; i < m_ImageViews.size(); ++i)
			{
				VkImageViewCreateInfo ImageViewInfo = {};
				ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				ImageViewInfo.image = m_Images[i];
				ImageViewInfo.format = pSwapchainManager->GetSwapchainH()->GetSurfaceFormat().format;
				ImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				ImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				ImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				ImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				ImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				ImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				ImageViewInfo.subresourceRange.baseMipLevel = 0;
				ImageViewInfo.subresourceRange.levelCount = 1;
				ImageViewInfo.subresourceRange.baseArrayLayer = 0;
				ImageViewInfo.subresourceRange.layerCount = 1;

				Debug::Result = vkCreateImageView(*pDevice, &ImageViewInfo, nullptr, &m_ImageViews[i]);
				CHECK(Debug::Result, "Image View Creation Failed");
			}
		}

		void ImageManager::DestroyResources() noexcept
		{
			for (std::size_t i = 0; i < m_ImageViews.size(); ++i)
			{
				VALIDATE_HANDLE(pDevice);
				VALIDATE_HANDLE(m_ImageViews[i]);

				vkDestroyImageView(*pDevice, m_ImageViews[i], nullptr);
			}

		}
	}
}