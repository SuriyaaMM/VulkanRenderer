
#pragma once

#include "Core.h"

namespace Fox
{
	namespace vk
	{
		/*
		* Initializes and Terminates GLFW Library
		* Handles the Creation and Destruction of VkInstance
		* Checks whether Required Extensions and Layers are available
		* Handles the Creation and Destruction of VkDebugUtilsMessengerKHR
		*/
		class InstanceManager : public Manager
		{
		public:

			InstanceManager(const InstanceManager& Source) = delete;
			InstanceManager(InstanceManager&& Source) = delete;

			InstanceManager(std::vector<const char*>* pRequiredExtensionsV,
				std::vector<const char*>* pRequiredLayersV);
			~InstanceManager() = default;

			virtual void DestroyResources() noexcept override;

			VkInstance* GetInstanceH() noexcept { return &m_Instance; }
			VkDebugUtilsMessengerEXT& GetDebugMessenger() noexcept { return m_DebugMessenger; }

		private:
			std::vector<const char*>* m_RInstanceExtensions;
			std::vector<const char*>* m_RInstanceLayers;

			std::vector<VkExtensionProperties>	m_AInstanceExtensionProperties;
			std::vector<VkLayerProperties>		m_AInstanceLayerProperties;

			VkInstance					m_Instance;
			VkDebugUtilsMessengerEXT	m_DebugMessenger;

			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);
		};
	}
}

