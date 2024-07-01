
#pragma once

#include "CommandBufferManager.h"
#include "MemoryManager.h"
#include "Buffer.h"

namespace Fox
{
	namespace vk
	{
		class ResourceManager : public Manager
		{
		public:
			ResourceManager(const ResourceManager& Other) = delete;
			ResourceManager(ResourceManager&& Other) = delete;

			ResourceManager(DeviceManager* pDeviceManager, MemoryManager* pMemoryManager);
			~ResourceManager() = default;

			virtual void DestroyResources() noexcept override;
			
			/*
			std::vector<Resource::Buffer>&	GetUniforms()					  noexcept 
			{ 
				return m_UniformBuffers; 
			}
			std::vector<void*>&				GetMappedMemoryRegionOfUniforms() noexcept
			{
				return m_MappedUniformMemory;
			}
			*/

		private:
			/*
			std::vector<Resource::Buffer> m_UniformBuffers;
			std::vector<void*> m_MappedUniformMemory;
			*/
		};
	}
}

