
#pragma once

#include "Core.h"

namespace Fox
{
	namespace Resource
	{
		/*
		* Non Copyable Resource
		* Constructor calls vkAllocateCommandPool()
		* DestroyResource() calls vkDestroyCommandPool() and also checks for validity of VkCommandPool
		*/
		class CommandPool : public MResource
		{
		public:
			CommandPool(const CommandPool& Other) = delete;
			CommandPool(CommandPool&& Other) = default;

			CommandPool(VkDevice* pValidDevice, uint32_t QueueFamilyIndex);
			~CommandPool() = default;

			virtual void DestroyResource() noexcept override;

			VkCommandPool* GetCommandPoolH() noexcept { return &m_CommandPool; }

		private:
			VkCommandPool m_CommandPool;
		};

		/*
		* Copyable Resource
		* DestroyResource() does nothing, Actual VkCommandBuffers are destroyed when the pool is destroyed
		*/
		class CommandBuffer : public MResource
		{
		public:
			CommandBuffer(const CommandBuffer& Other) = delete;
			CommandBuffer(CommandBuffer&& Other) = default;

			CommandBuffer(VkDevice* pValidDevice, CommandPool* pCommandPool, uint32_t BufferCount);
			~CommandBuffer() = default;

			virtual void DestroyResource() noexcept override;

			VkCommandBuffer* GetCommandBufferH() noexcept { return &m_CommandBuffer; }

		private:
			VkCommandBuffer m_CommandBuffer;
		};
	}
	namespace Utility
	{
		void BeginRecording(Resource::CommandBuffer* pCommandBuffer) noexcept;
		void EndRecording(Resource::CommandBuffer* pCommandBuffer) noexcept;
	}

	enum POOL_FAMILY
	{
		POOL_FAMILY_GRAPHICS = 0,
		POOL_FAMILY_TRANSFER
	};

	

}

