
#include "ResourceManager.h"

namespace Fox
{
	namespace vk
	{
		ResourceManager::ResourceManager(DeviceManager* pDeviceManager, MemoryManager* pMemoryManager)
			:
			Manager()
		{
			/*
			m_MappedUniformMemory.resize(MAX_ASYNC_FRAMES);
				
			for (std::size_t i = 0; i < MAX_ASYNC_FRAMES; ++i)
			{
				m_UniformBuffers.emplace_back(pMemoryManager->GetAllocatorH());
				m_UniformBuffers.back().CraftBuffer((uint32_t)sizeof(Uniform),
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_AUTO);

				vkMapMemory(*pDeviceManager->GetDeviceH(), 
					m_UniformBuffers[i].GetAllocationInfoMetaData()->deviceMemory, 0,
					(uint32_t)sizeof(Uniform), 0, &m_MappedUniformMemory[i]);
			}
			*/
		}

		void ResourceManager::DestroyResources() noexcept
		{
			/*
			for (std::size_t i = 0; i < m_UniformBuffers.size(); ++i)
			{
				m_UniformBuffers[i].DestroyResource();
			}
			*/
		}
	}
}