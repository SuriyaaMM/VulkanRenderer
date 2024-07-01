
#pragma once

#include "InstanceManager.h"

namespace Fox
{
	namespace vk
	{
		/*
		* Handles Creation and Destruction of GLFW Window's for Vulkan Runtime
		* Handles Creation and Destruction of Vulkan's Surfaces
		*/
		class WindowManager : public ILManager
		{
		public:
			WindowManager(const WindowManager& Source) = delete;
			WindowManager(WindowManager&& Source) = delete;

			WindowManager(uint32_t Width, uint32_t Height, std::string Title,
				VkInstance* pValidInstance);
			~WindowManager() = default;

			virtual void DestroyResources() noexcept override;

			GLFWwindow*		GetWindowH()				noexcept { return m_Window; }
			bool			HasFramebufferBeenResized() noexcept { return FramebufferBeenResized; }
			VkSurfaceKHR&	GetSurfaceH()				noexcept { return m_Surface; }

		private:
			GLFWwindow* m_Window;
			bool		FramebufferBeenResized;
			VkSurfaceKHR m_Surface;

			static void GLFWFramebufferResizeCallback(GLFWwindow* window, int width, int height);
		};
	}
}

