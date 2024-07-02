
#include "InstanceManager.h"

namespace Fox
{
	namespace vk
	{
		InstanceManager::InstanceManager(std::vector<const char*>* pRequiredExtensionsV,
			std::vector<const char*>* pRequiredLayersV)
			:
			Manager(),
			m_RInstanceExtensions(pRequiredExtensionsV), 
			m_RInstanceLayers(pRequiredLayersV),
			m_Instance(VK_NULL_HANDLE), 
			m_DebugMessenger(VK_NULL_HANDLE)
		{
			glfwInit();

			uint32_t RGLFWInstanceExtensionsCount = 0;
			const char** RGLFWInstanceExtensions = nullptr;

			RGLFWInstanceExtensions = glfwGetRequiredInstanceExtensions(
				&RGLFWInstanceExtensionsCount);

			for (uint32_t i = 0; i < RGLFWInstanceExtensionsCount; i++)
			{
				m_RInstanceExtensions->emplace_back(RGLFWInstanceExtensions[i]);
			}
			m_RInstanceExtensions->emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

			m_RInstanceLayers->emplace_back("VK_LAYER_KHRONOS_validation");

			//Section: Enumeration of Available Extensions

			uint32_t AInstanceExtensionPropertiesC = 0;

			vkEnumerateInstanceExtensionProperties(nullptr, &AInstanceExtensionPropertiesC,
				nullptr);

			m_AInstanceExtensionProperties =
				std::vector<VkExtensionProperties>(AInstanceExtensionPropertiesC);

			vkEnumerateInstanceExtensionProperties(nullptr, &AInstanceExtensionPropertiesC,
				m_AInstanceExtensionProperties.data());

			m_RInstanceExtensions->shrink_to_fit();
			m_AInstanceExtensionProperties.shrink_to_fit();

			//Section: Enumeration of Available Layers

			uint32_t AInstanceLayerPropertiesC = 0;

			vkEnumerateInstanceLayerProperties(&AInstanceLayerPropertiesC, nullptr);

			m_AInstanceLayerProperties =
				std::vector<VkLayerProperties>(AInstanceLayerPropertiesC);

			vkEnumerateInstanceLayerProperties(&AInstanceLayerPropertiesC,
				m_AInstanceLayerProperties.data());

			m_RInstanceLayers->shrink_to_fit();
			m_AInstanceLayerProperties.shrink_to_fit();

			for (std::size_t i = 0; i < m_RInstanceExtensions->size(); ++i)
			{
				bool Flag = false;

				for (std::size_t j = 0; j < m_AInstanceExtensionProperties.size(); ++j)
				{
					if (!strcmp(m_RInstanceExtensions->at(i),
						m_AInstanceExtensionProperties[j].extensionName))
					{
						Flag = true;
						break;
					}
				}

				if (Flag == false)
				{
					std::cout << "[Renderer]: Required Extension ||"
						<< m_RInstanceExtensions->at(i) << "|| is missing" << std::endl;
				}
			}
			for (std::size_t i = 0; i < m_RInstanceLayers->size(); ++i)
			{
				bool Flag = false;

				for (std::size_t j = 0; j < m_AInstanceLayerProperties.size(); ++j)
				{
					if (!strcmp(m_RInstanceLayers->at(i),
						m_AInstanceLayerProperties[j].layerName))
					{
						Flag = true;
						break;
					}
				}

				if (Flag == false)
				{
					std::cout << "[Renderer]: Required Layer ||"
						<< m_RInstanceLayers->at(i) << "|| is missing" << std::endl;
				}
			}

			//Section:: Filling of CreateInfo Structures

			VkDebugUtilsMessengerCreateInfoEXT DebugI = {};
			DebugI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			DebugI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			DebugI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			DebugI.pfnUserCallback = DebugCallback;
			DebugI.pUserData = nullptr;

			VkApplicationInfo ApplicationI = { };
			ApplicationI.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			ApplicationI.apiVersion = VK_API_VERSION_1_2;
			ApplicationI.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
			ApplicationI.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
			ApplicationI.pApplicationName = "VulkanRenderer";
			ApplicationI.pEngineName = "VulkanRenderer";
			ApplicationI.pNext = nullptr;

			VkInstanceCreateInfo InstanceI = {};
			InstanceI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceI.enabledExtensionCount = (uint32_t)m_RInstanceExtensions->size();
			InstanceI.enabledLayerCount = (uint32_t)m_RInstanceLayers->size();
			InstanceI.ppEnabledExtensionNames = m_RInstanceExtensions->data();
			InstanceI.ppEnabledLayerNames = m_RInstanceLayers->data();
			InstanceI.pApplicationInfo = &ApplicationI;
			InstanceI.flags = 0;
			InstanceI.pNext = nullptr;

			//Section:: Creation of Instance and Debug Messenger

			Debug::Result = vkCreateInstance(&InstanceI, nullptr, &m_Instance);
			CHECK(Debug::Result, "Instance Creation Failed!");

			PFN_vkCreateDebugUtilsMessengerEXT FNvkCreateDebugUtilsMessengerEXT
				= (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance,
					"vkCreateDebugUtilsMessengerEXT");

			Debug::Result = FNvkCreateDebugUtilsMessengerEXT(m_Instance, &DebugI,
				nullptr, &m_DebugMessenger);
			CHECK(Debug::Result, "Debug Messenger Creation Failed!");
		}

		void InstanceManager::DestroyResources() noexcept
		{
			VALIDATE_HANDLE(m_Instance);
			VALIDATE_HANDLE(m_DebugMessenger);

			PFN_vkDestroyDebugUtilsMessengerEXT FNDestroyDebugUtilsMessengerEXT
				= (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
					m_Instance, "vkDestroyDebugUtilsMessengerEXT");

			FNDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

			vkDestroyInstance(m_Instance, nullptr);

			glfwTerminate();
		}

		VKAPI_ATTR VkBool32 VKAPI_CALL InstanceManager::DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData)
		{
			switch (messageSeverity)
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				std::cerr << ANSI_RED << "[Validation]: "
					<< pCallbackData->pMessageIdName << "\n"
					<< "[Object Count]: " << pCallbackData->objectCount << "\n"
					<< "[Objects]: " << pCallbackData->pObjects << "\n"
					<< "[Queue Labels]: " << pCallbackData->pQueueLabels << "\n"
					<< "[Command Buffer Labels]: " << pCallbackData->pCmdBufLabels << "\n"
					<< "[Message]: \n" << pCallbackData->pMessage << ANSI_RESET << std::endl;
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				std::cerr << ANSI_YELLOW << "[Validation]: "
					<< pCallbackData->pMessageIdName << "\n"
					<< "[Object Count]: " << pCallbackData->objectCount << "\n"
					<< "[Objects]: " << pCallbackData->pObjects << "\n"
					<< "[Queue Labels]: " << pCallbackData->pQueueLabels << "\n"
					<< "[Command Buffer Labels]: " << pCallbackData->pCmdBufLabels << "\n"
					<< "[Message]: \n" << pCallbackData->pMessage << ANSI_RESET << std::endl;
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				std::cerr << ANSI_BLUE << "[Validation]: "
					<< pCallbackData->pMessageIdName << "\n"
					<< "[Object Count]: " << pCallbackData->objectCount << "\n"
					<< "[Objects]: " << pCallbackData->pObjects << "\n"
					<< "[Queue Labels]: " << pCallbackData->pQueueLabels << "\n"
					<< "[Command Buffer Labels]: " << pCallbackData->pCmdBufLabels << "\n"
					<< "[Message]: \n" << pCallbackData->pMessage << ANSI_RESET << std::endl;
				break;

			default:
				break;
			}

			return VK_FALSE;
		}
	}
}