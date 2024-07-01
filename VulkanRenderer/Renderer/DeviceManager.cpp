#include "DeviceManager.h"

namespace Fox
{
	namespace vk
	{
		DeviceManager::DeviceManager(VkInstance& Instance, VkSurfaceKHR& Surface,
			std::vector<const char*>* pRequiredExtensionsV,
			std::vector<const char*>* pRequiredLayersV)
			:
			Manager(),
			PhysicalDeviceManager::PhysicalDeviceManager(Instance),
			QueueManager::QueueManager(m_PhysicalDevice, Surface),
			m_RDeviceExtensions(pRequiredExtensionsV), 
			m_RDeviceLayers(pRequiredLayersV),
			m_Device(VK_NULL_HANDLE)
		{
			m_RDeviceExtensions->emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

			//Section:: Enumeration of Available Extensions 

			uint32_t ADeviceExtensionsC = 0;

			vkEnumerateDeviceExtensionProperties(m_PhysicalDevice,
				nullptr, &ADeviceExtensionsC, nullptr);

			m_ADeviceExtensionProperties =
				std::vector<VkExtensionProperties>(ADeviceExtensionsC);

			vkEnumerateDeviceExtensionProperties(m_PhysicalDevice,
				nullptr, &ADeviceExtensionsC, m_ADeviceExtensionProperties.data());

			//Section:: Enumeration of Available Layers

			uint32_t ADeviceLayersC = 0;

			vkEnumerateDeviceLayerProperties(m_PhysicalDevice,
				&ADeviceLayersC, nullptr);

			m_ADeviceLayerProperties =
				std::vector<VkLayerProperties>(ADeviceLayersC);

			vkEnumerateDeviceLayerProperties(m_PhysicalDevice,
				&ADeviceLayersC, m_ADeviceLayerProperties.data());

			m_RDeviceLayers->shrink_to_fit();
			m_RDeviceExtensions->shrink_to_fit();

			//Section:: Cross Validation of Required and Available Layers and Extensions

			for (std::size_t i = 0; i < m_RDeviceExtensions->size(); ++i)
			{
				bool Match = false;

				for (std::size_t j = 0; j < m_ADeviceExtensionProperties.size(); ++j)
				{
					if (!strcmp(m_RDeviceExtensions->at(i),
						m_ADeviceExtensionProperties[j].extensionName))
					{
						Match = true;
						break;
					}
				}

				if (Match != true)
				{
					std::cout << "[Renderer]: Required Device Extension ||"
						<< m_RDeviceExtensions->at(i) << "|| not Available!!" << std::endl;
				}
			}

			for (std::size_t i = 0; i < m_RDeviceLayers->size(); ++i)
			{
				bool Match = false;

				for (std::size_t j = 0; j < m_ADeviceLayerProperties.size(); ++j)
				{
					if (!strcmp(m_RDeviceLayers->at(i), m_ADeviceLayerProperties[j].layerName))
					{
						Match = true;
						break;
					}
				}

				if (Match != true)
				{
					std::cout << "[Renderer]: Required Device Layer ||"
						<< m_RDeviceLayers->at(i) << "|| not Available!!" << std::endl;
				}
			}

			//Section:: Filling of CreateInfo Structures

			VkDeviceQueueCreateInfo GraphicsQueueInfo = {};
			GraphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			GraphicsQueueInfo.queueFamilyIndex =
				(uint32_t)m_QueueFamilyIndices[QUEUE_FAMILY_GRAPHICS].value();
			GraphicsQueueInfo.queueCount = 1;
			GraphicsQueueInfo.pQueuePriorities =
				&m_QueueFamilyPriorities[QUEUE_FAMILY_GRAPHICS];
			GraphicsQueueInfo.flags = 0;
			GraphicsQueueInfo.pNext = nullptr;

			VkDeviceQueueCreateInfo TransferQueueI = {};
			TransferQueueI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			TransferQueueI.queueFamilyIndex =
				(uint32_t)m_QueueFamilyIndices[QUEUE_FAMILY_TRANSFER].value();
			TransferQueueI.queueCount = 1;
			TransferQueueI.pQueuePriorities =
				&m_QueueFamilyPriorities[QUEUE_FAMILY_TRANSFER];
			TransferQueueI.flags = 0;
			TransferQueueI.pNext = nullptr;

			std::array<VkDeviceQueueCreateInfo, 2> QueueCreateInfos = {};

			QueueCreateInfos[0] = GraphicsQueueInfo;
			QueueCreateInfos[1] = TransferQueueI;

			//Section:: Creation of Logical Device

			VkDeviceCreateInfo DeviceInfo = {};
			DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			DeviceInfo.enabledExtensionCount = (uint32_t)m_RDeviceExtensions->size();
			DeviceInfo.ppEnabledExtensionNames = m_RDeviceExtensions->data();
			DeviceInfo.enabledLayerCount = (uint32_t)m_RDeviceLayers->size();
			DeviceInfo.ppEnabledLayerNames = m_RDeviceLayers->data();
			DeviceInfo.pQueueCreateInfos = QueueCreateInfos.data();
			DeviceInfo.queueCreateInfoCount = (uint32_t)QueueCreateInfos.size();
			DeviceInfo.pEnabledFeatures = &m_EnabledPhysicalDeviceFeatures;
			DeviceInfo.flags = 0;
			DeviceInfo.pNext = nullptr;

			Debug::Result = vkCreateDevice(m_PhysicalDevice, &DeviceInfo, nullptr, &m_Device);
			CHECK(Debug::Result, "Device Creation Failed!");

			//Section:: Fetching Valid Queue Handles

			vkGetDeviceQueue(m_Device,
				m_QueueFamilyIndices[QUEUE_FAMILY_GRAPHICS].value(), 0,
				&m_QueueHandles.at(QUEUE_FAMILY_GRAPHICS));

			vkGetDeviceQueue(m_Device,
				m_QueueFamilyIndices[QUEUE_FAMILY_TRANSFER].value(), 0,
				&m_QueueHandles.at(QUEUE_FAMILY_TRANSFER));
		}

		void DeviceManager::DestroyResources() noexcept
		{
			VALIDATE_HANDLE(m_Device);
			vkDestroyDevice(m_Device, nullptr);
		}
	}
}