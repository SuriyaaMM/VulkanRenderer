
#include "Application.h"

namespace Fox
{
	Application::Application()
		:m_VulkanManager()
	{
		while (!glfwWindowShouldClose(m_VulkanManager.GetWindowManager().GetWindowH()))
		{
			glfwPollEvents();

			if (m_VulkanManager.GetWindowManager().HasFramebufferBeenResized())
			{
				m_VulkanManager.RecreateFramebuffers();
			}

			m_VulkanManager.DrawFrame();
		}

		vkDeviceWaitIdle(*m_VulkanManager.GetDeviceHManager().GetDeviceH());
	}
}