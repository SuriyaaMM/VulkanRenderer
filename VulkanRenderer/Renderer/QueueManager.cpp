#include "QueueManager.h"

namespace Fox
{
	namespace vk
	{
		QueueManager::QueueManager(VkPhysicalDevice& PhysicalDevice,
			VkSurfaceKHR& Surface)
			:m_QueueHandles(), m_QueueFamilyIndices(), m_QueueFamilyPriorities()
		{
			uint32_t AQueueFamiliesC = 0;

			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &AQueueFamiliesC, nullptr);

			m_QueueFamilyProperties =
				std::vector<VkQueueFamilyProperties>(AQueueFamiliesC);
			m_QueueHandles =
				std::vector<VkQueue>(AQueueFamiliesC);

			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &AQueueFamiliesC,
				m_QueueFamilyProperties.data());

			m_QueueFamilyPriorities[QUEUE_FAMILY_GRAPHICS] = 1.0f;
			m_QueueFamilyPriorities[QUEUE_FAMILY_TRANSFER] = 1.0f;


			std::vector<uint32_t> GraphicsFamilyIndices = {};

			std::vector<uint32_t> TransferFamilyIndices = {};


			for (std::size_t i = 0; i < m_QueueFamilyProperties.size(); ++i)
			{
				if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					GraphicsFamilyIndices.emplace_back((uint32_t)i);
				}
				if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
				{
					TransferFamilyIndices.emplace_back((uint32_t)i);
				}

				VkBool32 HasPresentationSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, (uint32_t)i, Surface,
					&HasPresentationSupport);

				if (!HasPresentationSupport)
				{
					CHECK(VK_ERROR_OUT_OF_HOST_MEMORY, "Device Doesn't Support Presentation");
				}
			}

			GraphicsFamilyIndices.shrink_to_fit();
			TransferFamilyIndices.shrink_to_fit();

			for (std::size_t i = 0; i < GraphicsFamilyIndices.size(); ++i)
			{
				for (std::size_t j = 0; j < TransferFamilyIndices.size(); ++j)
				{
					if (GraphicsFamilyIndices[i] != TransferFamilyIndices[j])
					{
						m_QueueFamilyIndices[QUEUE_FAMILY_GRAPHICS] = (uint32_t)i;
						m_QueueFamilyIndices[QUEUE_FAMILY_TRANSFER] = (uint32_t)j;
					}
				}
			}

			if (!m_QueueFamilyIndices[QUEUE_FAMILY_GRAPHICS].has_value() ||
				!m_QueueFamilyIndices[QUEUE_FAMILY_TRANSFER].has_value())
			{
				for (std::size_t i = 0; i < m_QueueFamilyIndices.size(); ++i)
				{
					if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT
						&& m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
					{
						m_QueueFamilyIndices[QUEUE_FAMILY_GRAPHICS] = (uint32_t)i;
						m_QueueFamilyIndices[QUEUE_FAMILY_TRANSFER] = (uint32_t)i;
					}
					else
					{
						CHECK(VK_ERROR_VALIDATION_FAILED_EXT, "Transfer Queue isn't Available!");
					}
				}
			}
		}

		void QueueManager::DestroyResources() noexcept
		{

		}

		void QueueManager::SetQueueFamilyPriority(QUEUE_FAMILY QueueFamily,
			float Priority) noexcept
		{
			m_QueueFamilyPriorities[QueueFamily] = Priority;
		}
	}
}