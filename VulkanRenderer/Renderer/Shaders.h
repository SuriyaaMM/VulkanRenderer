
#pragma once

#include "DeviceManager.h"

namespace Fox
{
	namespace Resource
	{
		class Shader : public MResource
		{
		public:
			Shader(const Shader& Other) = delete;
			Shader(Shader&& Other) = delete;

			Shader(vk::DeviceManager* pDeviceManager, VkShaderStageFlagBits ShaderStage, 
				std::string&& SpirvFilePath);
			~Shader() = default;

			virtual void DestroyResource() noexcept override;

			VkShaderModule& GetShaderModule() noexcept { return m_ShaderModule; }
			VkPipelineShaderStageCreateInfo& GetShaderStageCreateI() noexcept
			{
				return m_ShaderCreateI;
			}

		private:

			VkShaderModule					m_ShaderModule;
			VkPipelineShaderStageCreateInfo m_ShaderCreateI;
		};
	}

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

