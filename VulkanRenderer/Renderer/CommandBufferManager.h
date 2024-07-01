
#pragma once

#include "PipelineManager.h"
#include "FramebufferManager.h"

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

	namespace vk
	{
		class CommandBufferManager : public Manager
		{
		public:
			CommandBufferManager(const CommandBufferManager& Other) = delete;
			CommandBufferManager(CommandBufferManager&& Other) = delete;

			CommandBufferManager(DeviceManager* pDeviceManager);
			~CommandBufferManager() = default;

			_NODISCARD Resource::CommandBuffer* CreateCommandBuffer(VkDevice* pValidDevice, 
				POOL_FAMILY PoolFamily, uint32_t&& Count) noexcept;

			Resource::CommandBuffer* GetCommandBufferHAt(uint32_t Index)
			{
				return &m_CommandBuffers.at(Index);
			}
			Resource::CommandBuffer* GetCoreTransferCommandBufferH() noexcept
			{
				return &m_CommandBuffers[MAX_ASYNC_FRAMES];
			}

			virtual void DestroyResources() noexcept override;

		private:
			Resource::CommandPool m_GraphicsPool;
			Resource::CommandPool m_TransferPool;

			std::vector<Resource::CommandBuffer> m_CommandBuffers;
		};
	}

}

