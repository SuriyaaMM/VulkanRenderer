
#pragma once

#include "SwapchainManager.h"

namespace Fox
{
	namespace vk
	{
		class ImageManager : public DLManager
		{
		public:

			ImageManager(const ImageManager& Source) = delete;
			ImageManager(ImageManager&& Source) = delete;

			ImageManager(DeviceManager* pDeviceManager,
				SwapchainManager* pSwapchainManager);
			~ImageManager() = default;

			virtual void DestroyResources() noexcept override;

			std::vector<VkImageView>&	GetImageViewsV()	noexcept { return m_ImageViews; }
			std::vector<VkImage>&		GetImageHandlesV()	noexcept { return m_Images; }

		private:

			std::vector<VkImage>		m_Images;
			std::vector<VkImageView>	m_ImageViews;
		};
	}
}

