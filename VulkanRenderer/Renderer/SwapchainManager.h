
#pragma once

#include "DeviceManager.h"
#include "WindowManager.h"

namespace Fox
{
	namespace Resource
	{
		class Swapchain : public CResource
		{
		public:
			Swapchain(const Swapchain& Other) = default;
			Swapchain(Swapchain&& Other) = default;
			Swapchain& operator = (const Swapchain& Other) = default;

			Swapchain(vk::DeviceManager* pDeviceManager, vk::WindowManager* pWindowManager);
			~Swapchain() = default;

			virtual void DestroyResource() noexcept override;

			uint32_t&			GetImageCount()		noexcept { return m_ImageCount; }
			VkPresentModeKHR&	GetPresentMode()	noexcept { return m_PresentMode; }
			VkSurfaceFormatKHR&	GetSurfaceFormat()	noexcept { return m_SurfaceFormat; }
			VkExtent2D&			GetExtent()			noexcept { return m_Extent; }
			VkSwapchainKHR*		GetSwapchainH()		noexcept { return &m_Swapchain; }

			std::vector<VkImage>& GetImageVH() noexcept { return m_Images; }
			std::vector<VkImageView>& GetImageViewVH() noexcept { return m_ImageViews; }

		private:
			uint32_t m_ImageCount;

			std::vector<VkSurfaceFormatKHR>		m_ASurfaceFormats;
			std::vector<VkPresentModeKHR>		m_APresentModes;

			VkSwapchainCreateInfoKHR					m_SwapchainCreateI;
			VkSurfaceCapabilitiesKHR					m_SurfaceCapabilities;
			VkSurfaceFormatKHR							m_SurfaceFormat;
			VkPresentModeKHR							m_PresentMode;
			VkExtent2D									m_Extent;
			VkSwapchainKHR								m_Swapchain;

			std::vector<VkImage>				m_Images;
			std::vector<VkImageView>			m_ImageViews;
		};
	}

	namespace vk
	{
		class SwapchainManager : public Manager
		{
		public:

			SwapchainManager(const SwapchainManager& Other) = delete;
			SwapchainManager(SwapchainManager&& Other) = delete;

			SwapchainManager(DeviceManager* pDeviceManager, WindowManager* pWindowManager);
			~SwapchainManager() = default;

			void ReconstructSwapchain(DeviceManager* pDeviceManager, WindowManager* pWindowManager);

			virtual void DestroyResources() noexcept override;

			Resource::Swapchain* GetSwapchainH() noexcept { return &m_Swapchain; }
			
		private:
			Resource::Swapchain m_Swapchain;
		};
	}
}

