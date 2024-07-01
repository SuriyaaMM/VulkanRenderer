#include "Buffer.h"

namespace Fox
{
	namespace Resource
	{
		/*
		Buffer::Buffer(VkDevice* pValidDevice, VkPhysicalDevice* pValidPhysicalDevice,
			VkBufferUsageFlags BufferUsageFlags, uint32_t Size, VkMemoryPropertyFlags MemoryFlags)
			:
			MResource(pValidDevice),
			m_Memory(VK_NULL_HANDLE), m_Buffer(VK_NULL_HANDLE)
		{
			VkBufferCreateInfo BufferI = {};
			BufferI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferI.size = Size;
			BufferI.usage = BufferUsageFlags;
			BufferI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			BufferI.flags = 0;
			BufferI.pNext = nullptr;
			BufferI.pQueueFamilyIndices = nullptr;
			BufferI.queueFamilyIndexCount = 0;

			Debug::Result = vkCreateBuffer(*pDevice, &BufferI, nullptr, &m_Buffer);
			CHECK(Debug::Result, "Buffer Creation Failed!");

			VkMemoryRequirements BufferMemoryRequirements = {};
			vkGetBufferMemoryRequirements(*pDevice, m_Buffer, &BufferMemoryRequirements);

			VulkanMemoryManager::Allocate(*pDevice, BufferMemoryRequirements.size,
				Utility::FetchRightMemoryTypeIndex(*pValidPhysicalDevice,
					BufferMemoryRequirements.memoryTypeBits, MemoryFlags), &m_Memory);

			Debug::Result = vkBindBufferMemory(*pDevice, m_Buffer, m_Memory, 0);
			CHECK(Debug::Result, "Buffer Handle to Memory Binding Failed!");
		}

		void Buffer::StageBuffer(vk::DeviceManager* pDeviceManager,
			CommandBuffer* pTransferCommandBuffer, Resource::Buffer* pStagingBuffer,
			void* Data, uint32_t DataSize, VkDeviceSize SourceOffset)
		{
			void* MappedData = nullptr;

			vkMapMemory(*pDevice, *pStagingBuffer->GetBufferMemoryH(), 0, DataSize, 0, &MappedData);
			memcpy(MappedData, Data, DataSize);
			vkUnmapMemory(*pDevice, *pStagingBuffer->GetBufferMemoryH());

			VkBufferCopy CopyRegion = {};
			CopyRegion.dstOffset = 0;
			CopyRegion.srcOffset = 0;
			CopyRegion.size = DataSize;

			Utility::BeginRecording(pTransferCommandBuffer);

			vkCmdCopyBuffer(*pTransferCommandBuffer->GetCommandBufferH(), *pStagingBuffer->GetBufferH(),
				m_Buffer, 1, &CopyRegion);

			Utility::EndRecording(pTransferCommandBuffer);

			VkSubmitInfo TransferSubmitInfo = {};
			TransferSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			TransferSubmitInfo.commandBufferCount = 1;
			TransferSubmitInfo.pCommandBuffers = pTransferCommandBuffer->GetCommandBufferH();
			TransferSubmitInfo.pNext = nullptr;
			TransferSubmitInfo.pSignalSemaphores = nullptr;
			TransferSubmitInfo.pWaitDstStageMask = nullptr;
			TransferSubmitInfo.pWaitSemaphores = nullptr;
			TransferSubmitInfo.signalSemaphoreCount = 0;
			TransferSubmitInfo.waitSemaphoreCount = 0;

			vkQueueSubmit(pDeviceManager->GetQueue(QUEUE_FAMILY_TRANSFER), 1, &TransferSubmitInfo,
				VK_NULL_HANDLE);
			vkQueueWaitIdle(pDeviceManager->GetQueue(QUEUE_FAMILY_TRANSFER));
		}

		void Buffer::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(m_Buffer);
			VALIDATE_HANDLE(m_Memory);

			vkFreeMemory(*pDevice, m_Memory, nullptr);
			vkDestroyBuffer(*pDevice, m_Buffer, nullptr);
		}

		*/

		Buffer::Buffer(VmaAllocator* pValidAllocator)
			:
			GPUResource(pValidAllocator),
			m_Buffer(VK_NULL_HANDLE),
			m_AllocationMD(VK_NULL_HANDLE), m_AllocationInfoMD()
		{
			
		}

		void Buffer::CraftBuffer(uint32_t Size, VkBufferUsageFlags BufferUsage, 
			VmaAllocationCreateFlags AllocationFlags, VmaMemoryUsage MemoryUsage) 
			noexcept
		{
			VkBufferCreateInfo BufferCreateI = {};
			BufferCreateI.flags = 0;
			BufferCreateI.pNext = nullptr;
			BufferCreateI.pQueueFamilyIndices = nullptr;
			BufferCreateI.queueFamilyIndexCount = 0;
			BufferCreateI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			BufferCreateI.size = Size;
			BufferCreateI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			BufferCreateI.usage = BufferUsage;

			VmaAllocationCreateInfo VmaAllocationCreateI = {};
			VmaAllocationCreateI.flags = AllocationFlags;
			VmaAllocationCreateI.memoryTypeBits = 0;
			VmaAllocationCreateI.pool = 0;
			VmaAllocationCreateI.preferredFlags = 0;
			VmaAllocationCreateI.priority = 0;
			VmaAllocationCreateI.pUserData = nullptr;
			VmaAllocationCreateI.requiredFlags = 0;
			VmaAllocationCreateI.usage = MemoryUsage;

			Debug::Result = vmaCreateBuffer(*pAllocator, &BufferCreateI, &VmaAllocationCreateI, &m_Buffer,
				&m_AllocationMD, &m_AllocationInfoMD);
			CHECK(Debug::Result, "Vertex Buffer Creation Failed!");
		}

		void Buffer::StageBuffer(void* pData, VkDeviceSize DataSize, 
			CommandBuffer* pTransferCommandBuffer, VkQueue* pQueue) noexcept
		{			
			Buffer StagingBuffer(pAllocator);
			StagingBuffer.CraftBuffer((uint32_t)DataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
				VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
				VMA_MEMORY_USAGE_CPU_ONLY);

			void* pMappedData = nullptr;

			vmaMapMemory(*pAllocator, *StagingBuffer.GetAllocationMetaData(), &pMappedData);

			memcpy(pMappedData, pData, (std::size_t)DataSize);

			vmaUnmapMemory(*pAllocator, *StagingBuffer.GetAllocationMetaData());

			VkBufferCopy CopyRegion = {};
			CopyRegion.dstOffset = 0;
			CopyRegion.srcOffset = 0;
			CopyRegion.size = DataSize;

			Utility::BeginRecording(pTransferCommandBuffer);

			vkCmdCopyBuffer(*pTransferCommandBuffer->GetCommandBufferH(),
				*StagingBuffer.GetBufferH(), m_Buffer, 1, &CopyRegion);

			Utility::EndRecording(pTransferCommandBuffer);

			VkSubmitInfo TransferSubmitInfo = {};
			TransferSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			TransferSubmitInfo.commandBufferCount = 1;
			TransferSubmitInfo.pCommandBuffers = pTransferCommandBuffer->GetCommandBufferH();
			TransferSubmitInfo.pNext = nullptr;
			TransferSubmitInfo.pSignalSemaphores = nullptr;
			TransferSubmitInfo.pWaitDstStageMask = nullptr;
			TransferSubmitInfo.pWaitSemaphores = nullptr;
			TransferSubmitInfo.signalSemaphoreCount = 0;
			TransferSubmitInfo.waitSemaphoreCount = 0;

			vkQueueSubmit(*pQueue, 1, &TransferSubmitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(*pQueue);

			StagingBuffer.DestroyResource();
		}

		void Buffer::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(m_Buffer);
			VALIDATE_HANDLE(m_AllocationMD);
			vmaDestroyBuffer(*pAllocator, m_Buffer, m_AllocationMD);
		}
	}
}