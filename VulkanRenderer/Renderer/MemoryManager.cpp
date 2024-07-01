#include "MemoryManager.h"

namespace Fox
{
	namespace vk
	{
		MemoryManager::MemoryManager(InstanceManager* pInstanceManager, DeviceManager* pDeviceManager)
			:
			DLManager(pDeviceManager->GetDeviceH()),
			m_Allocator(VK_NULL_HANDLE)
		{
			VmaAllocatorCreateInfo AllocatorCreateI = {};
			AllocatorCreateI.device = *pDeviceManager->GetDeviceH();
			AllocatorCreateI.flags = 0;
			AllocatorCreateI.instance = *pInstanceManager->GetInstanceH();
			AllocatorCreateI.pAllocationCallbacks = nullptr;
			AllocatorCreateI.pDeviceMemoryCallbacks = nullptr;
			AllocatorCreateI.pHeapSizeLimit = nullptr;
			AllocatorCreateI.physicalDevice = *pDeviceManager->GetPhysicalDeviceH();
			AllocatorCreateI.preferredLargeHeapBlockSize = 0;
			AllocatorCreateI.pTypeExternalMemoryHandleTypes = nullptr;
			AllocatorCreateI.pVulkanFunctions = nullptr;
			AllocatorCreateI.vulkanApiVersion = VK_API_VERSION_1_2;

			Debug::Result = vmaCreateAllocator(&AllocatorCreateI, &m_Allocator);
			CHECK(Debug::Result, "VMA Allocator Creation Failed!");
		}

		void MemoryManager::DestroyResources() noexcept
		{
			VALIDATE_HANDLE(m_Allocator);
			vmaDestroyAllocator(m_Allocator);
		}
	}
}