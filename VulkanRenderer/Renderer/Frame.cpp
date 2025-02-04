
#include "Frame.h"

namespace Fox
{
	namespace Resource
	{
		Frame::Frame(vk::DeviceManager* pDeviceManager, vk::WindowManager* pWindowManager,
			vk::CommandPoolManager* pCommandPoolManager)
			: 
			MResource(pDeviceManager->GetDeviceH()),
			m_CommandBuffer(pDevice, pCommandPoolManager->GetGraphicsPoolH(), 1),
			m_RenderFinished(pDevice, VK_SEMAPHORE_TYPE_BINARY),
			m_ImageAvailable(pDevice, VK_SEMAPHORE_TYPE_BINARY),
			m_AsyncFence(pDevice, VK_FENCE_CREATE_SIGNALED_BIT)
		{
			
		}

		void Frame::PresentFrame(Swapchain* pSwapchain, VkQueue* pGraphicsQueue, uint32_t ImageIndex) 
			noexcept
		{
			VkPipelineStageFlags WaitStages[1] =
			{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			VkSubmitInfo QueueSubmitI = {};
			QueueSubmitI.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			QueueSubmitI.pWaitSemaphores = m_ImageAvailable.GetSemaphoreH();
			QueueSubmitI.pWaitDstStageMask = WaitStages;
			QueueSubmitI.commandBufferCount = 1;
			QueueSubmitI.pCommandBuffers = m_CommandBuffer.GetCommandBufferH();
			QueueSubmitI.pSignalSemaphores = m_RenderFinished.GetSemaphoreH();
			QueueSubmitI.signalSemaphoreCount = 1;
			QueueSubmitI.waitSemaphoreCount = 1;
			QueueSubmitI.pNext = nullptr;

			Debug::Result = vkQueueSubmit(*pGraphicsQueue, 1, &QueueSubmitI, *m_AsyncFence.GetFenceH());
			CHECK(Debug::Result, "Queue Submission Failed!");

			VkPresentInfoKHR PresentI = {};
			PresentI.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			PresentI.pNext = nullptr;
			PresentI.pResults = nullptr;
			PresentI.pSwapchains = pSwapchain->GetSwapchainH();
			PresentI.pWaitSemaphores = m_RenderFinished.GetSemaphoreH();
			PresentI.swapchainCount = 1;
			PresentI.waitSemaphoreCount = 1;
			PresentI.pImageIndices = &ImageIndex;

			vkQueuePresentKHR(*pGraphicsQueue, &PresentI);
		}

		void Frame::DestroyResource() noexcept
		{
			m_AsyncFence.DestroyResource();
			m_ImageAvailable.DestroyResource();
			m_RenderFinished.DestroyResource();
		}
	}
}