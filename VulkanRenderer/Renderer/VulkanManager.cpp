#include "VulkanManager.h"

namespace Fox
{
	namespace vk
	{
		VulkanManager::VulkanManager()
			:
			m_BackbufferIndex(0),
			m_RInstanceExtensions(0), m_RInstanceLayers(0),
			m_RDeviceExtensions(0), m_RDeviceLayers(0),

			m_InstanceManager(&m_RInstanceExtensions, &m_RInstanceLayers),
			m_WindowManager(600, 400, "Vulkan Renderer", m_InstanceManager.GetInstanceH()),
			m_DeviceManager(*m_InstanceManager.GetInstanceH(), m_WindowManager.GetSurfaceH(),
				&m_RDeviceExtensions, &m_RDeviceLayers),
			m_MemoryManager(&m_InstanceManager, &m_DeviceManager),

			m_Frames(),
			
			m_SwapchainManager(&m_DeviceManager, &m_WindowManager),
			m_PipelineManager(&m_DeviceManager, m_SwapchainManager.GetSwapchainH()->GetSurfaceFormat()),
			m_CommandBufferManager(&m_DeviceManager),
			m_SyncManager(&m_DeviceManager),
			m_ResourceManager(&m_DeviceManager, &m_MemoryManager),
			/*
			m_UniformLayouts(MAX_ASYNC_FRAMES, m_PipelineManager.GetUniformDescriptor()),
			m_UniformDescriptors(MAX_ASYNC_FRAMES),
			*/
			m_TriangleMesh(m_DeviceManager.GetDeviceH(), m_MemoryManager.GetAllocatorH(), 
				m_CommandBufferManager.GetCoreTransferCommandBufferH(),
				&m_DeviceManager.GetQueue(QUEUE_FAMILY_TRANSFER))
		{
			m_pImageManager = std::make_unique<ImageManager>(&m_DeviceManager, &m_SwapchainManager);
			m_pFramebufferManager = std::make_unique<FramebufferManager>(&m_DeviceManager,
				&m_SwapchainManager, &m_PipelineManager, m_pImageManager.get());

			m_PipelineManager.SetViewportAndScissor(m_SwapchainManager.GetSwapchainH()->GetExtent());

			for (std::size_t i = 0; i < MAX_ASYNC_FRAMES; ++i)
			{
				m_Frames.emplace_back(&m_DeviceManager, &m_WindowManager);
			}

			/*
			VkDescriptorPoolSize PoolSize = {};
			PoolSize.descriptorCount = (uint32_t)MAX_ASYNC_FRAMES;
			PoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

			VkDescriptorPoolCreateInfo PoolCreateI = {};
			PoolCreateI.flags = 0;
			PoolCreateI.maxSets = (uint32_t)MAX_ASYNC_FRAMES;
			PoolCreateI.pNext = nullptr;
			PoolCreateI.poolSizeCount = 1;
			PoolCreateI.pPoolSizes = &PoolSize;
			PoolCreateI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

			Debug::Result = vkCreateDescriptorPool(*m_DeviceManager.GetDeviceH(), &PoolCreateI,
				nullptr, &m_DescriptorPool);
			CHECK(Debug::Result, "Descriptor Pool Creation Failed!");

			VkDescriptorSetAllocateInfo DescriptorAllocateI = {};
			DescriptorAllocateI.descriptorPool = m_DescriptorPool;
			DescriptorAllocateI.descriptorSetCount = (uint32_t)MAX_ASYNC_FRAMES;
			DescriptorAllocateI.pNext = nullptr;
			DescriptorAllocateI.pSetLayouts = m_UniformLayouts.data();
			DescriptorAllocateI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

			Debug::Result = vkAllocateDescriptorSets(*m_DeviceManager.GetDeviceH(),
				&DescriptorAllocateI, m_UniformDescriptors.data());
			CHECK(Debug::Result, "Descriptor Set Allocation Failed!");

			for (std::size_t i = 0; i < MAX_ASYNC_FRAMES; ++i)
			{
				VkDescriptorBufferInfo DescriptorBufferI = {};
				DescriptorBufferI.buffer = *m_ResourceManager.GetUniforms()[i].GetBufferH();
				DescriptorBufferI.offset = 0;
				DescriptorBufferI.range = sizeof(Uniform);

				VkWriteDescriptorSet DescriptorWrite = {};
				DescriptorWrite.descriptorCount = 1;
				DescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				DescriptorWrite.dstArrayElement = 0;
				DescriptorWrite.dstBinding = 0;
				DescriptorWrite.dstSet = m_UniformDescriptors[i];
				DescriptorWrite.pBufferInfo = &DescriptorBufferI;
				DescriptorWrite.pImageInfo = nullptr;
				DescriptorWrite.pNext = nullptr;
				DescriptorWrite.pTexelBufferView = nullptr;
				DescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;

				vkUpdateDescriptorSets(*m_DeviceManager.GetDeviceH(), 1, &DescriptorWrite, 0, nullptr);
			}
			*/

			Utility::BeginRecording(m_CommandBufferManager.GetCommandBufferHAt(m_BackbufferIndex));

			RecordDrawCommand(
				m_pFramebufferManager->GetFramebufferVH().at(m_BackbufferIndex).GetFramebufferH(),
				m_CommandBufferManager.GetCommandBufferHAt(m_BackbufferIndex));

			Utility::EndRecording(m_CommandBufferManager.GetCommandBufferHAt(m_BackbufferIndex));
		}

		VulkanManager::~VulkanManager()
		{
			vkDestroyDescriptorPool(*m_DeviceManager.GetDeviceH(), m_DescriptorPool, nullptr);
			
			m_TriangleMesh.DestroyResources();
			m_ResourceManager.DestroyResources();
			m_SyncManager.DestroyResources();
			m_CommandBufferManager.DestroyResources();
			m_pFramebufferManager->DestroyResources();
			m_PipelineManager.DestroyResources();
			m_pImageManager->DestroyResources();
			m_SwapchainManager.DestroyResources();
			for (std::size_t i = 0; i < m_Frames.size(); ++i)
			{
				m_Frames[i].DestroyResource();
			}
			m_MemoryManager.DestroyResources();
			m_WindowManager.DestroyResources();
			m_DeviceManager.DestroyResources();
			m_InstanceManager.DestroyResources();
		}

		void VulkanManager::DrawFrame() noexcept
		{
			m_Frames[m_BackbufferIndex].PresentFrame(m_SwapchainManager.GetSwapchainH(),
				&m_pFramebufferManager->GetFramebufferVH()[m_BackbufferIndex],
				m_CommandBufferManager.GetCommandBufferHAt(m_BackbufferIndex),
				&m_DeviceManager.GetQueue(QUEUE_FAMILY_GRAPHICS));
		}

		void VulkanManager::RecreateFramebuffers() noexcept
		{
			vkDeviceWaitIdle(*m_DeviceManager.GetDeviceH());

			m_SwapchainManager.RecreateSwapchain(&m_DeviceManager, &m_WindowManager);

			m_pImageManager->DestroyResources();

			m_pImageManager = std::make_unique<ImageManager>(&m_DeviceManager, &m_SwapchainManager);

			m_pFramebufferManager->DestroyResources();

			m_pFramebufferManager = std::make_unique<FramebufferManager>(&m_DeviceManager,
				&m_SwapchainManager, &m_PipelineManager, m_pImageManager.get());

			m_PipelineManager.SetViewportAndScissor(m_SwapchainManager.GetSwapchainH()->GetExtent());
		}

		void VulkanManager::RecordDrawCommand(VkFramebuffer* pFramebuffer,
			Resource::CommandBuffer* pCommandBuffer) noexcept
		{
			VkClearValue ClearValue = { {{0.3f, 0.2f, 0.4f, 1.0f}} };

			VkRenderPassBeginInfo RenderPassBeginI = {};
			RenderPassBeginI.clearValueCount = 1;
			RenderPassBeginI.framebuffer = *pFramebuffer;
			RenderPassBeginI.pClearValues = &ClearValue;
			RenderPassBeginI.pNext = nullptr;
			RenderPassBeginI.renderPass = *m_PipelineManager.GetRenderPassH();
			RenderPassBeginI.renderArea.offset = { 0,0 };
			RenderPassBeginI.renderArea.extent = m_SwapchainManager.GetSwapchainH()->GetExtent();
			RenderPassBeginI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

			vkCmdBeginRenderPass(*pCommandBuffer->GetCommandBufferH(), &RenderPassBeginI,
				VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(*pCommandBuffer->GetCommandBufferH(), VK_PIPELINE_BIND_POINT_GRAPHICS,
				*m_PipelineManager.GetPipelineH());

			vkCmdSetViewport(*pCommandBuffer->GetCommandBufferH(), 0, 1, &m_PipelineManager.GetViewport());
			vkCmdSetScissor(*pCommandBuffer->GetCommandBufferH(), 0, 1, &m_PipelineManager.GetScissor());

			m_TriangleMesh.Draw(reinterpret_cast<void*>(pCommandBuffer));

			/*
			vkCmdBindDescriptorSets(*pCommandBuffer->GetCommandBufferH(), VK_PIPELINE_BIND_POINT_GRAPHICS,
				m_PipelineManager.GetPipelineLayout(), 0, 1,
				&m_UniformDescriptors[m_BackbufferIndex], 0, nullptr);
				*/

			vkCmdEndRenderPass(*pCommandBuffer->GetCommandBufferH());
		}

		void VulkanManager::UpdateUniform(uint32_t ImageIndex) noexcept
		{
			static auto StartTime = std::chrono::high_resolution_clock::now();

			auto CurrentTime = std::chrono::high_resolution_clock::now();

			float time = std::chrono::duration<float,
				std::chrono::seconds::period>(CurrentTime - StartTime).count();

			Fox::Uniform UBO = {};

			UBO.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
				glm::vec3(0.0f, 0.0f, 1.0f));

			UBO.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f));

			UBO.Projection = glm::perspective(glm::radians(45.0f),
				m_SwapchainManager.GetSwapchainH()->GetExtent().width / (float)m_SwapchainManager.GetSwapchainH()->GetExtent().height,
				0.1f, 10.0f);
			UBO.Projection[1][1] *= -1;

			/*
			memcpy(m_ResourceManager.GetMappedMemoryRegionOfUniforms()[ImageIndex],
				&UBO, sizeof(UBO));
				*/
		}
	}
}