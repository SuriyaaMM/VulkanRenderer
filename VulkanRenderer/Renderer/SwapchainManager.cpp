#include "SwapchainManager.h"

namespace Fox
{
	namespace Resource
	{
		Swapchain::Swapchain(vk::DeviceManager* pDeviceManager, vk::WindowManager* pWindowManager)
			:
			CResource(pDeviceManager->GetDeviceH())
		{
			//Section:: Querying for Swapchain Details

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*pDeviceManager->GetPhysicalDeviceH(),
				pWindowManager->GetSurfaceH(), &m_SurfaceCapabilities);

			uint32_t AFormatsC = 0;

			vkGetPhysicalDeviceSurfaceFormatsKHR(*pDeviceManager->GetPhysicalDeviceH(),
				pWindowManager->GetSurfaceH(), &AFormatsC, nullptr);

			m_ASurfaceFormats =
				std::vector<VkSurfaceFormatKHR>(AFormatsC);

			vkGetPhysicalDeviceSurfaceFormatsKHR(*pDeviceManager->GetPhysicalDeviceH(),
				pWindowManager->GetSurfaceH(), &AFormatsC, m_ASurfaceFormats.data());

			uint32_t APresentModesC = 0;

			vkGetPhysicalDeviceSurfacePresentModesKHR(*pDeviceManager->GetPhysicalDeviceH(),
				pWindowManager->GetSurfaceH(), &APresentModesC, nullptr);

			m_APresentModes =
				std::vector<VkPresentModeKHR>(APresentModesC);

			vkGetPhysicalDeviceSurfacePresentModesKHR(*pDeviceManager->GetPhysicalDeviceH(),
				pWindowManager->GetSurfaceH(), &APresentModesC, m_APresentModes.data());

			//Section:: Validation of Swapchain

			if (m_ASurfaceFormats.empty() || m_APresentModes.empty())
			{
				std::cout << ANSI_YELLOW <<
					"[Renderer]: Swapchain isn't Suitable for Rendering!"
					<< ANSI_RESET << std::endl;
			}

			//Section:: Selection of Format & Presentation Mode

			for (std::size_t i = 0; i < m_ASurfaceFormats.size(); ++i)
			{
				if (m_ASurfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB
					&& m_ASurfaceFormats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
				{
					m_SurfaceFormat = m_ASurfaceFormats[i];
				}
			}

			for (std::size_t i = 0; i < m_APresentModes.size(); ++i)
			{
				if (m_APresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					m_PresentMode = m_APresentModes[i];
					break;
				}
				else
				{
					m_PresentMode = VK_PRESENT_MODE_FIFO_KHR;
				}
			}

			//Section:: Choosing Surface Extent

			if (m_SurfaceCapabilities.currentExtent.width
				!= std::numeric_limits<uint32_t>::max())
			{
				m_Extent = m_SurfaceCapabilities.currentExtent;
			}
			else
			{
				int Width = 0, Height = 0;
				glfwGetFramebufferSize(pWindowManager->GetWindowH(), &Width, &Height);

				m_Extent = VkExtent2D();

				m_Extent.width = std::clamp((uint32_t)Width,
					m_SurfaceCapabilities.minImageExtent.width,
					m_SurfaceCapabilities.maxImageExtent.width);

				m_Extent.height = std::clamp((uint32_t)Height,
					m_SurfaceCapabilities.minImageExtent.height,
					m_SurfaceCapabilities.maxImageExtent.height);
			}

			//Section:: Swapchain Creation

			uint32_t ImageCount = 0;

			if (!(m_SurfaceCapabilities.maxImageCount < MAX_ASYNC_FRAMES))
			{
				ImageCount = MAX_ASYNC_FRAMES;
			}
			else
			{
				ImageCount = m_SurfaceCapabilities.minImageCount;
			}

			m_ImageCount = ImageCount;

			m_SwapchainCreateI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			m_SwapchainCreateI.surface = pWindowManager->GetSurfaceH();
			m_SwapchainCreateI.imageFormat = m_SurfaceFormat.format;
			m_SwapchainCreateI.imageColorSpace = m_SurfaceFormat.colorSpace;
			m_SwapchainCreateI.presentMode = m_PresentMode;
			m_SwapchainCreateI.imageArrayLayers = 1;
			m_SwapchainCreateI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			m_SwapchainCreateI.minImageCount = ImageCount;
			m_SwapchainCreateI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			m_SwapchainCreateI.queueFamilyIndexCount = 0;
			m_SwapchainCreateI.pQueueFamilyIndices = nullptr;

			/*
			if (0)
			{
				m_SwapchainCreateI.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				m_SwapchainCreateI.queueFamilyIndexCount = 2;
				m_SwapchainCreateI.pQueueFamilyIndices =
					pDeviceManager->GetQueueFamilyIndicesPacked().data();
			}
			else
			{
				m_SwapchainCreateI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				m_SwapchainCreateI.queueFamilyIndexCount = 0;
				m_SwapchainCreateI.pQueueFamilyIndices = nullptr;
			}
			*/

			m_SwapchainCreateI.imageExtent = m_Extent;
			m_SwapchainCreateI.preTransform = m_SurfaceCapabilities.currentTransform;
			m_SwapchainCreateI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			m_SwapchainCreateI.clipped = true;
			m_SwapchainCreateI.oldSwapchain = nullptr;
			m_SwapchainCreateI.pNext = nullptr;

			Debug::Result = vkCreateSwapchainKHR(*pDevice, &m_SwapchainCreateI, nullptr, &m_Swapchain);
			CHECK(Debug::Result, "Swapchain Creation Failed!");

			vkGetSwapchainImagesKHR(*pDevice, m_Swapchain, &ImageCount, nullptr);

			m_Images.resize(ImageCount);

			vkGetSwapchainImagesKHR(*pDevice, m_Swapchain, &ImageCount, m_Images.data());

			m_ImageViews.resize(m_Images.size());

			for (std::size_t i = 0; i < m_ImageViews.size(); ++i)
			{
				VkImageViewCreateInfo ImageViewInfo = {};
				ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				ImageViewInfo.image = m_Images[i];
				ImageViewInfo.format = m_SurfaceFormat.format;
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

		void Swapchain::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(pDevice);
			VALIDATE_HANDLE(m_Swapchain);

			for (std::size_t i = 0; i < m_ImageViews.size(); ++i)
			{
				VALIDATE_HANDLE(pDevice);
				VALIDATE_HANDLE(m_ImageViews[i]);

				vkDestroyImageView(*pDevice, m_ImageViews[i], nullptr);
			}

			vkDestroySwapchainKHR(*pDevice, m_Swapchain, nullptr);
		}
	}

	namespace vk
	{
		SwapchainManager::SwapchainManager(DeviceManager* pDeviceManager,
			WindowManager* pWindowManager)
			:
			Manager(),
			m_Swapchain(pDeviceManager, pWindowManager)
		{

		}

		void SwapchainManager::ReconstructSwapchain(DeviceManager* pDeviceManager, 
			WindowManager* pWindowManager)
		{
			m_Swapchain.DestroyResource();
			m_Swapchain = Resource::Swapchain(pDeviceManager, pWindowManager);
		}

		void SwapchainManager::DestroyResources() noexcept
		{
			m_Swapchain.DestroyResource();
		}
	}
}