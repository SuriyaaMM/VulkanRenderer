#include "CommandPoolManager.h"

namespace Fox
{
	namespace vk
	{
		CommandPoolManager::CommandPoolManager(DeviceManager* pDeviceManager)
			:
			m_GraphicsPool(pDeviceManager->GetDeviceH(),
				pDeviceManager->GetQueueFamilyIndex(QUEUE_FAMILY_GRAPHICS)),
			m_TransferPool(pDeviceManager->GetDeviceH(),
				pDeviceManager->GetQueueFamilyIndex(QUEUE_FAMILY_TRANSFER))
		{

		}

		void CommandPoolManager::DestroyResources() noexcept
		{
			m_TransferPool.DestroyResource();
			m_GraphicsPool.DestroyResource();
		}
	}
}