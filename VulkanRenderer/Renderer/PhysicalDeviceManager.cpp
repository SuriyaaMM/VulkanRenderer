#include "PhysicalDeviceManager.h"

namespace Fox
{
	namespace vk
	{
		PhysicalDeviceManager::PhysicalDeviceManager(VkInstance& Instance)
		{
			//Section:: Enumeration of Available Physical Devices

			uint32_t APhysicalDeviceC = 0;

			vkEnumeratePhysicalDevices(Instance, &APhysicalDeviceC, nullptr);

			m_APhysicalDevices = std::vector<VkPhysicalDevice>(APhysicalDeviceC);
			m_APhysicalDeviceFeatures = std::vector<VkPhysicalDeviceFeatures>(APhysicalDeviceC);
			m_APhysicalDeviceProperties = std::vector<VkPhysicalDeviceProperties>(APhysicalDeviceC);

			vkEnumeratePhysicalDevices(Instance, &APhysicalDeviceC, m_APhysicalDevices.data());

			for (std::size_t i = 0; i < m_APhysicalDevices.size(); ++i)
			{
				VkPhysicalDeviceFeatures PhysicalDeviceFeatures = {};
				vkGetPhysicalDeviceFeatures(m_APhysicalDevices[i], &PhysicalDeviceFeatures);
				m_APhysicalDeviceFeatures.push_back(PhysicalDeviceFeatures);

				VkPhysicalDeviceProperties PhysicalDeviceProperties = {};
				vkGetPhysicalDeviceProperties(m_APhysicalDevices[i], &PhysicalDeviceProperties);
				m_APhysicalDeviceProperties.push_back(PhysicalDeviceProperties);
			}

			//Temporarily Chosing the First GPU as our Physical Device
			m_PhysicalDevice = m_APhysicalDevices[0];
			m_EnabledPhysicalDeviceFeatures = {};
		}
	}
	
}