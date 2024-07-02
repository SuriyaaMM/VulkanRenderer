
#pragma once

#include "Core.h"

namespace Fox
{
	namespace vk
	{
		class PhysicalDeviceManager : public Manager
		{
		public:
			PhysicalDeviceManager(const PhysicalDeviceManager& Source) = delete;
			PhysicalDeviceManager(PhysicalDeviceManager&& Source) = delete;

			PhysicalDeviceManager(VkInstance& Instance);
			~PhysicalDeviceManager() = default;

			VkPhysicalDeviceFeatures& GetEnabledPhysicalDeviceFeatures() noexcept
			{
				return m_EnabledPhysicalDeviceFeatures;
			}
			VkPhysicalDevice* GetPhysicalDeviceH() noexcept { return &m_PhysicalDevice; }

			virtual void DestroyResources() noexcept override;

		protected:
			std::vector<VkPhysicalDevice>			m_APhysicalDevices;
			std::vector<VkPhysicalDeviceFeatures>	m_APhysicalDeviceFeatures;
			std::vector<VkPhysicalDeviceProperties> m_APhysicalDeviceProperties;

			VkPhysicalDevice						m_PhysicalDevice;
			VkPhysicalDeviceFeatures				m_EnabledPhysicalDeviceFeatures;
		};
	}
}

