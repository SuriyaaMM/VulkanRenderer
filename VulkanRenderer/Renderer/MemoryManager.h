
#pragma once

#include "InstanceManager.h"
#include "DeviceManager.h"

namespace Fox
{
	namespace vk
	{
		class MemoryManager : public DLManager
		{
		public:
			MemoryManager(const MemoryManager& Other) = delete;
			MemoryManager(MemoryManager&& Other) = delete;

			MemoryManager(InstanceManager* pInstanceManager,DeviceManager* pDeviceManager);
			~MemoryManager() = default;

			virtual void DestroyResources() noexcept override;

			VmaAllocator* GetAllocatorH() noexcept { return &m_Allocator; }

		private:
			VmaAllocator m_Allocator;
		};
	}
}

