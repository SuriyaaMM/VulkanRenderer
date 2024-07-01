#include "CommandBufferManager.h"

namespace Fox
{
	namespace Resource
	{
		CommandPool::CommandPool(VkDevice* pValidDevice, uint32_t QueueFamilyIndex)
			:
			MResource(pValidDevice),
			m_CommandPool(VK_NULL_HANDLE)
		{
			VkCommandPoolCreateInfo CommandPoolI = {};
			CommandPoolI.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			CommandPoolI.pNext = nullptr;
			CommandPoolI.queueFamilyIndex = QueueFamilyIndex;
			CommandPoolI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

			Debug::Result = vkCreateCommandPool(*pDevice, &CommandPoolI, nullptr, &m_CommandPool);
			CHECK(Debug::Result, "Graphics Command Pool Creation Failed!");
		}

		void CommandPool::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(pDevice);
			VALIDATE_HANDLE(m_CommandPool);
			vkDestroyCommandPool(*pDevice, m_CommandPool, nullptr);
		}

		CommandBuffer::CommandBuffer(VkDevice* pValidDevice, CommandPool* pCommandPool, 
			uint32_t BufferCount)
			:
			MResource(pValidDevice),
			m_CommandBuffer(VK_NULL_HANDLE)
		{
			VkCommandBufferAllocateInfo CommandBufferI = {};
			CommandBufferI.commandBufferCount = BufferCount;
			CommandBufferI.commandPool = *pCommandPool->GetCommandPoolH();
			CommandBufferI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			CommandBufferI.pNext = nullptr;
			CommandBufferI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

			Debug::Result = vkAllocateCommandBuffers(*pDevice, &CommandBufferI, &m_CommandBuffer);
			CHECK(Debug::Result, "Graphics Command Buffer Allocation Failed!");
		}

		void CommandBuffer::DestroyResource() noexcept
		{

		}
	}

	namespace Utility
	{
		void BeginRecording(Resource::CommandBuffer* pCommandBuffer) noexcept
		{
			VkCommandBufferBeginInfo CommandBufferBeginI = {};
			CommandBufferBeginI.flags = 0;
			CommandBufferBeginI.pInheritanceInfo = nullptr;
			CommandBufferBeginI.pNext = nullptr;
			CommandBufferBeginI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			Debug::Result = vkBeginCommandBuffer(*pCommandBuffer->GetCommandBufferH(), 
				&CommandBufferBeginI);
			CHECK(Debug::Result, "Initiation of Command Recording Failed!");
		}

		void EndRecording(Resource::CommandBuffer* pCommandBuffer) noexcept
		{
			Debug::Result = vkEndCommandBuffer(*pCommandBuffer->GetCommandBufferH());
			CHECK(Debug::Result, "Ending Command Recording Failed!");
		}
	}

	namespace vk
	{
		CommandBufferManager::CommandBufferManager(DeviceManager* pDeviceManager)
			:
			m_GraphicsPool(pDeviceManager->GetDeviceH(), pDeviceManager->GetQueueFamilyIndex(QUEUE_FAMILY_GRAPHICS)),
			m_TransferPool(pDeviceManager->GetDeviceH(), pDeviceManager->GetQueueFamilyIndex(QUEUE_FAMILY_TRANSFER)),
			m_CommandBuffers()
		{
			//Section:: Creation of Command Buffers from Graphics Pool
			//Creates MAX_ASYNC_FRAMES number of Graphics Pool Compatible Command Buffers
			for (std::size_t i = 0; i < MAX_ASYNC_FRAMES; ++i)
			{
				m_CommandBuffers.emplace_back(std::move(
					Resource::CommandBuffer(pDeviceManager->GetDeviceH(), &m_GraphicsPool, 1)));
			}

			//Section:: Creation of Command Buffer from Transfer Pool
			m_CommandBuffers.emplace_back(std::move(
				Resource::CommandBuffer(pDeviceManager->GetDeviceH(), &m_TransferPool, 1)));
		}

		_NODISCARD Resource::CommandBuffer* CommandBufferManager::CreateCommandBuffer(
			VkDevice* pValidDevice, POOL_FAMILY PoolFamily, uint32_t&& Count) noexcept
		{
			if (PoolFamily == POOL_FAMILY_GRAPHICS)
			{
				m_CommandBuffers.emplace_back(std::move(
					Resource::CommandBuffer(pValidDevice, &m_GraphicsPool, Count)));
				return &m_CommandBuffers.back();
			}
			else if (PoolFamily == POOL_FAMILY_TRANSFER)
			{
				m_CommandBuffers.emplace_back(std::move(
					Resource::CommandBuffer(pValidDevice, &m_TransferPool, Count)));
				return &m_CommandBuffers.back();
			}
			else
			{
				CHECK(VK_ERROR_OUT_OF_HOST_MEMORY, "Queue Family Unknown");
				return 0;
			}
		}

		void CommandBufferManager::DestroyResources() noexcept
		{
			for (std::size_t i = 0; i < m_CommandBuffers.size(); ++i)
			{
				m_CommandBuffers[i].DestroyResource();
			}

			m_TransferPool.DestroyResource();
			m_GraphicsPool.DestroyResource();
		}
	}

}