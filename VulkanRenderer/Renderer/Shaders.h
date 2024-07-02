
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

			VkShaderModule& GetShaderModule() noexcept 
			{ 
				return m_ShaderModule; 
			}
			VkPipelineShaderStageCreateInfo& GetShaderStageCreateI() noexcept
			{
				return m_ShaderCreateI;
			}

		private:
			VkShaderModule					m_ShaderModule;
			VkPipelineShaderStageCreateInfo m_ShaderCreateI;
		};
	}
}

