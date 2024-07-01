
#pragma once

#include "QueueManager.h"
#include "PhysicalDeviceManager.h"

namespace Fox
{
	/*
	* Handles Creation and Destruction of VkDevice Object
	* Initializes and Destructs PhysicalDeviceManager
	* Initializes and Destructs QueueManager
	* Fill's in Valid Queue Handles for Respective Queues
	*/
	namespace vk
	{
		class DeviceManager : public PhysicalDeviceManager, public QueueManager, public Manager
		{
		public:
			DeviceManager(const DeviceManager& Source) = delete;
			DeviceManager(DeviceManager&& Source) = default;

			DeviceManager(VkInstance& Instance, VkSurfaceKHR& Surface,
				std::vector<const char*>* RequiredExtensions,
				std::vector<const char*>* RequiredLayers);
			~DeviceManager() = default;

			virtual void DestroyResources() noexcept;

			VkDevice* GetDeviceH() noexcept { return &m_Device; }

		private:
			std::vector<const char*>* m_RDeviceExtensions;
			std::vector<const char*>* m_RDeviceLayers;
			std::vector<VkExtensionProperties> m_ADeviceExtensionProperties;
			std::vector<VkLayerProperties> m_ADeviceLayerProperties;

			VkDevice m_Device;
		};
	}
}

