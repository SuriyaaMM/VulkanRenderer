#include "ShaderManager.h"

namespace Fox
{
	namespace vk
	{
		ShaderManager::ShaderManager(DeviceManager* pDeviceManager)
			:
			m_VertexShader(pDeviceManager, VK_SHADER_STAGE_VERTEX_BIT,
				"Renderer\\Shaders\\VertexShader.spirv"),
			m_PixelShader(pDeviceManager, VK_SHADER_STAGE_FRAGMENT_BIT,
				"Renderer\\Shaders\\PixelShader.spirv")
		{

		}

		void ShaderManager::DestroyResources() noexcept
		{
			m_PixelShader.DestroyResource();
			m_VertexShader.DestroyResource();
		}
	}
}