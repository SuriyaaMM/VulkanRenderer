
#pragma once

#include "Renderer/VulkanManager.h"

namespace Fox
{
	class Application
	{
	public:

		Application(const Application& Source) = delete;
		Application(Application&& Source) = delete;

		Application();

	private:
		
		Fox::vk::VulkanManager m_VulkanManager;
	};
}

