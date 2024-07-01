#include "SyncManager.h"

namespace Fox
{
	namespace Resource
	{
		Semaphore::Semaphore(VkDevice* pValidDevice, VkSemaphoreCreateFlags CreateFlags)
			:
			MResource(pValidDevice)
		{
			VkSemaphoreCreateInfo SemaphoreI = {};
			SemaphoreI.flags = CreateFlags;
			SemaphoreI.pNext = nullptr;
			SemaphoreI.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			Debug::Result = vkCreateSemaphore(*pDevice, &SemaphoreI, nullptr, &m_Semaphore);
			CHECK(Debug::Result, "Semaphore Creation Failed!");
		}

		void Semaphore::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(pDevice);
			VALIDATE_HANDLE(m_Semaphore);

			vkDestroySemaphore(*pDevice, m_Semaphore, nullptr);
		}

		Fence::Fence(VkDevice* pValidDevice, VkFenceCreateFlags CreateFlags)
			:MResource(pValidDevice)
		{
			VkFenceCreateInfo FenceI = {};
			FenceI.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			FenceI.pNext = nullptr;
			FenceI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

			Debug::Result = vkCreateFence(*pDevice, &FenceI, nullptr, &m_Fence);
			CHECK(Debug::Result, "Fence Creation Failed!");
		}

		void Fence::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(pDevice);
			VALIDATE_HANDLE(m_Fence);

			vkDestroyFence(*pDevice, m_Fence, nullptr);
		}
	}

	namespace vk
	{
		SyncManager::SyncManager(DeviceManager* pDeviceManager)
			: 
			Manager(),
			m_ImageAvailable(), m_RenderFinished(), m_AsyncFence()
		{
			for (std::size_t i = 0; i < MAX_ASYNC_FRAMES; ++i)
			{
				m_ImageAvailable.emplace_back(pDeviceManager->GetDeviceH(), VK_SEMAPHORE_TYPE_BINARY);
				m_RenderFinished.emplace_back(pDeviceManager->GetDeviceH(), VK_SEMAPHORE_TYPE_BINARY);
				m_AsyncFence.emplace_back(pDeviceManager->GetDeviceH(), VK_FENCE_CREATE_SIGNALED_BIT);
			}
		}

		void SyncManager::RecreateSyncObjects(DeviceManager* pDeviceManager)
		{
			for (std::size_t i = 0; i < MAX_ASYNC_FRAMES; ++i)
			{
				m_ImageAvailable[i].DestroyResource();
			}
			for (std::size_t i = 0; i < MAX_ASYNC_FRAMES; ++i)
			{
				m_RenderFinished[i].DestroyResource();
			}
			m_ImageAvailable.clear();
			m_RenderFinished.clear();
			
			
			for (std::size_t i = 0; i < MAX_ASYNC_FRAMES; ++i)
			{
				m_ImageAvailable.emplace_back(pDeviceManager->GetDeviceH(), VK_SEMAPHORE_TYPE_BINARY);
				m_RenderFinished.emplace_back(pDeviceManager->GetDeviceH(), VK_SEMAPHORE_TYPE_BINARY);
			}
		}

		void SyncManager::DestroyResources() noexcept
		{
			for (std::size_t i = 0; i < m_ImageAvailable.size(); ++i)
			{
				m_ImageAvailable[i].DestroyResource();
			}
			for (std::size_t i = 0; i < m_RenderFinished.size(); ++i)
			{
				m_RenderFinished[i].DestroyResource();
			}
			for (std::size_t i = 0; i < m_AsyncFence.size(); ++i)
			{
				m_AsyncFence[i].DestroyResource();
			}
		}
	}

}