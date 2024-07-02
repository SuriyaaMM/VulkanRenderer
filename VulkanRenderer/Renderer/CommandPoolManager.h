
#pragma once

#include "Commandbuffer.h"
#include "DeviceManager.h"

namespace Fox
{
	namespace vk
	{
		class CommandPoolManager : public Manager
		{
		public:
			CommandPoolManager(const CommandPoolManager& Other) = delete;
			CommandPoolManager(CommandPoolManager&& Other) = delete;

			CommandPoolManager(DeviceManager* pDeviceManager);
			~CommandPoolManager() = default;

			virtual void DestroyResources() noexcept override;

			Resource::CommandPool* GetGraphicsPoolH() noexcept { return &m_GraphicsPool; }
			Resource::CommandPool* GetTransferPoolH() noexcept { return &m_TransferPool; }

		private:
			Resource::CommandPool m_GraphicsPool;
			Resource::CommandPool m_TransferPool;
		};

	}
}
