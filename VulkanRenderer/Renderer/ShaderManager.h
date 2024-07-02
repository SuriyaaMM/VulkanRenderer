
#pragma once

#include "Shaders.h"

namespace Fox
{
	namespace vk
	{
		class ShaderManager : public Manager
		{
		public:

			ShaderManager(const ShaderManager& Other) = delete;
			ShaderManager(ShaderManager&& Other) = delete;

			ShaderManager(DeviceManager* pDeviceManager);
			~ShaderManager() = default;

			Resource::Shader& GetPixelShaderH() noexcept { return m_PixelShader; }
			Resource::Shader& GetVertexShaderH() noexcept { return m_VertexShader; }

			virtual void DestroyResources() noexcept override;

		private:
			Resource::Shader m_VertexShader;
			Resource::Shader m_PixelShader;
		};
	}
}

