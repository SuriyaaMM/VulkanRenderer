
#pragma once

#include "DeviceManager.h"
#include "WindowManager.h"

namespace Fox
{
	namespace Resource
	{
		class Swapchain : public MResource
		{
		public:
			Swapchain(const Swapchain& Other) = delete;
			Swapchain(Swapchain&& Other) = delete;

			Swapchain(vk::DeviceManager* pDeviceManager, vk::WindowManager* pWindowManager);
			~Swapchain() = default;

			virtual void DestroyResource() noexcept override;

			uint32_t&			GetImageCount()		noexcept { return m_ImageCount; }
			VkPresentModeKHR&	GetPresentMode()	noexcept { return m_PresentMode; }
			VkSurfaceFormatKHR&	GetSurfaceFormat()	noexcept { return m_SurfaceFormat; }
			VkExtent2D&			GetExtent()			noexcept { return m_Extent; }
			VkSwapchainKHR*		GetSwapchainH()		noexcept { return &m_Swapchain; }

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

			void RecreateSwapchain(DeviceManager* pDeviceManager, WindowManager* pWindowManager);

			virtual void DestroyResources() noexcept override;

			Resource::Swapchain* GetSwapchainH() noexcept { return m_pSwapchain.get(); }
			
		private:
			std::unique_ptr<Resource::Swapchain> m_pSwapchain;
		};
	}
}

