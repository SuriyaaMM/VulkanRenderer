#pragma once

#include "MemoryManager.h"
#include "CommandBuffer.h"

namespace Fox
{
	namespace Resource
	{
		class Buffer : public GPUResource
		{
		public:
			Buffer(const Buffer& Other) = delete;
			Buffer(Buffer&& Other) = default;

			Buffer(VmaAllocator* pValidAllocator);
			~Buffer() = default;

			void CraftBuffer(uint32_t Size, VkBufferUsageFlags BufferUsage,
				VmaAllocationCreateFlags AllocationFlags, VmaMemoryUsage MemoryUsage)
				noexcept;
			void StageBuffer(void* pData, VkDeviceSize DataSize,
				CommandBuffer* pTransferCommandBuffer, VkQueue* pQueue) noexcept;

			virtual void DestroyResource() noexcept override;

			VkBuffer*			GetBufferH()				noexcept { return &m_Buffer; }
			VmaAllocation*		GetAllocationMetaData()		noexcept { return &m_AllocationMD; }
			VmaAllocationInfo*	GetAllocationInfoMetaData() noexcept { return &m_AllocationInfoMD; }

		private:
			VkBuffer			m_Buffer;
			VmaAllocation		m_AllocationMD;
			VmaAllocationInfo	m_AllocationInfoMD;
		};

		/*
		class Buffer : public MResource
		{
		public:
			Buffer(const Buffer& Other) = delete;
			Buffer(Buffer&& Other) = default;

			Buffer(VkDevice* pValidDevice, VkPhysicalDevice* pValidPhysicalDevice,
				VkBufferUsageFlags BufferUsageFlags, uint32_t Size,
				VkMemoryPropertyFlags MemoryFlags);

			~Buffer() = default;

			void StageBuffer(vk::DeviceManager* pDeviceManager,
				CommandBuffer* pTransferCommandBuffer, Resource::Buffer* pStagingBuffer,
				void* Data, uint32_t DataSize, VkDeviceSize SourceOffset);

			virtual void DestroyResource() noexcept override;

			VkBuffer*		GetBufferH()		noexcept { return &m_Buffer; }
			VkDeviceMemory* GetBufferMemoryH()	noexcept { return &m_Memory; }

		private:
			VkBuffer		m_Buffer;
			VkDeviceMemory	m_Memory;
		};
		*/
	}
}