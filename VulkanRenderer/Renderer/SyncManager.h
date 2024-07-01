
#pragma once

#include "DeviceManager.h"

namespace Fox
{
	namespace Resource
	{
		class Semaphore : public MResource
		{
		public:
			Semaphore(const Semaphore& Other) = delete;
			Semaphore(Semaphore&& Other) = default;

			Semaphore(VkDevice* PValidDevice, VkSemaphoreCreateFlags CreateFlags);
			~Semaphore() = default;

			VkSemaphore* GetSemaphoreH() noexcept { return &m_Semaphore; }

			virtual void DestroyResource() noexcept override;

		private:
			VkSemaphore m_Semaphore;
		};

		class Fence : public MResource
		{
		public:
			Fence(const Fence& Other) = delete;
			Fence(Fence&& Other) = default;

			Fence(VkDevice* pValidDevice, VkFenceCreateFlags CreateFlags);
			~Fence() = default;

			VkFence* GetFenceH() noexcept { return &m_Fence; }

			virtual void DestroyResource() noexcept override;

		private:
			VkFence m_Fence;
		};
	}

	namespace vk
	{
		class SyncManager : public Manager
		{
		public:

			SyncManager(const SyncManager& Other) = delete;
			SyncManager(SyncManager&& Other) = delete;

			SyncManager(DeviceManager* pDeviceManager);
			~SyncManager() = default;

			void RecreateSyncObjects(DeviceManager* pDeviceManager);

			virtual void DestroyResources() noexcept override;

			Resource::Semaphore* GetImageAvailableSemaphore() noexcept { return m_ImageAvailable.data(); }
			Resource::Semaphore* GetRenderFinishedSemaphore() noexcept { return m_RenderFinished.data(); }
			Resource::Fence*	 GetFence()					  noexcept { return m_AsyncFence.data(); }

		private:
			std::vector<Resource::Semaphore> m_ImageAvailable;
			std::vector<Resource::Semaphore> m_RenderFinished;
			std::vector<Resource::Fence> m_AsyncFence;
		};
	}
}

