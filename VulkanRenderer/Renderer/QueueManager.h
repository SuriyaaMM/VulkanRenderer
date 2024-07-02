
#pragma once

#include "Core.h"

namespace Fox
{
	enum QUEUE_FAMILY
	{
		QUEUE_FAMILY_GRAPHICS,
		QUEUE_FAMILY_TRANSFER
	};

	namespace vk
	{
		class QueueManager : public Manager
		{
		public:
			QueueManager(const QueueManager& Source) = delete;
			QueueManager(QueueManager&& Source) = delete;

			QueueManager(VkPhysicalDevice& PhysicalDevice, VkSurfaceKHR& Surface);
			~QueueManager() = default;

			virtual void DestroyResources() noexcept override;

			void SetQueueFamilyPriority(QUEUE_FAMILY QueueFamily, float Priority) noexcept;

			std::array<uint32_t, 2> GetQueueFamilyIndicesPacked() noexcept
			{
				return std::array<uint32_t, 2>(
					{	m_QueueFamilyIndices[QUEUE_FAMILY_GRAPHICS].value(),
						m_QueueFamilyIndices[QUEUE_FAMILY_TRANSFER].value() });
			}

			uint32_t& GetQueueFamilyIndex(QUEUE_FAMILY QueueFamily) noexcept
			{
				return m_QueueFamilyIndices[QueueFamily].value();
			}

			VkQueue* GetQueueH(QUEUE_FAMILY QueueFamily) noexcept
			{
				return &m_QueueHandles[QueueFamily];
			}

			float&		GetQueueFamilyPriority(QUEUE_FAMILY QueueFamily) noexcept
			{
				return m_QueueFamilyPriorities[QueueFamily];
			}

		protected:
			std::vector<VkQueue>										m_QueueHandles;
			std::unordered_map<QUEUE_FAMILY, std::optional<uint32_t>>	m_QueueFamilyIndices;
			std::unordered_map<QUEUE_FAMILY, float>						m_QueueFamilyPriorities;
			std::vector<VkQueueFamilyProperties>						m_QueueFamilyProperties;
		};
	}
}

