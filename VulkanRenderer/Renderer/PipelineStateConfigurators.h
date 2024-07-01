
#pragma once

#include "Core.h"

namespace Fox
{
	namespace Config
	{
		class VertexInputConfig
		{
		public:
			VertexInputConfig(const VertexInputConfig& Other) = delete;
			VertexInputConfig(VertexInputConfig&& Other) = delete;

			VertexInputConfig();
			~VertexInputConfig() = default;

			VkPipelineVertexInputStateCreateInfo* GetStateCreateInfo() noexcept
			{
				return &m_VertexInputI;
			}

		private:
			uint32_t m_VertexAttributeC;
			std::vector<VkVertexInputAttributeDescription> m_VertexInputAttributeDescriptions;

			VkVertexInputBindingDescription m_BindingDescription;
			VkPipelineVertexInputStateCreateInfo m_VertexInputI;
		};

		class InputAssemblyConfig
		{
		public:
			InputAssemblyConfig(const InputAssemblyConfig& Other) = delete;
			InputAssemblyConfig(InputAssemblyConfig&& Other) = delete;

			InputAssemblyConfig();
			~InputAssemblyConfig() = default;

			VkPipelineInputAssemblyStateCreateInfo* GetStateCreateInfo() noexcept
			{
				return &m_IAStateCreateI;
			}

		private:
			VkPipelineInputAssemblyStateCreateInfo m_IAStateCreateI;
		};
	}
}

