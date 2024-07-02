#include "SyncResources.h"

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
}