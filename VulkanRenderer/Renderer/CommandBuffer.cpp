#include "CommandBuffer.h"

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

	

}