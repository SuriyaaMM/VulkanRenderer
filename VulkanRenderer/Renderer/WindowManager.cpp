#include "WindowManager.h"

namespace Fox
{
	namespace vk
	{
		WindowManager::WindowManager(uint32_t Width, uint32_t Height,
			std::string Title, VkInstance* pValidInstance)
			:
			ILManager(pValidInstance),
			m_Window(VK_NULL_HANDLE), m_Surface(VK_NULL_HANDLE)
		{
			//Section:: Creation of GLFW Window
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			m_Window = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);

			if (m_Window == nullptr)
			{
				std::cout << "GLFW Window Creation Failed!" << std::endl;
				std::terminate();
			}

			glfwSetFramebufferSizeCallback(m_Window, GLFWFramebufferResizeCallback);
			glfwSetWindowUserPointer(m_Window, this);

			//Section:: Creation of Vulkan Surface for the Window

			VkWin32SurfaceCreateInfoKHR SurfaceInfo = {};
			SurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			SurfaceInfo.hwnd = glfwGetWin32Window(m_Window);
			SurfaceInfo.hinstance = GetModuleHandleA(nullptr);
			SurfaceInfo.flags = 0;
			SurfaceInfo.pNext = nullptr;

			Debug::Result = vkCreateWin32SurfaceKHR(*pInstance, &SurfaceInfo, nullptr, &m_Surface);
			CHECK(Debug::Result, "Win32 Surface Creation Failed!");
		}

		void WindowManager::DestroyResources() noexcept
		{
			VALIDATE_HANDLE(pInstance);
			VALIDATE_HANDLE(m_Surface);
			VALIDATE_HANDLE(m_Window);

			vkDestroySurfaceKHR(*pInstance, m_Surface, nullptr);
			glfwDestroyWindow(m_Window);
		}

		void WindowManager::GLFWFramebufferResizeCallback(GLFWwindow* window, int width, int height)
		{
			WindowManager* This = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
			This->FramebufferBeenResized = true;
		}
	}
}