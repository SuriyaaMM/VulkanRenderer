
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

		class DynamicStatesConfig
		{
		public:
			DynamicStatesConfig(const DynamicStatesConfig& Other) = delete;
			DynamicStatesConfig(DynamicStatesConfig&& Other) = delete;

			DynamicStatesConfig();
			~DynamicStatesConfig() = default;

			VkPipelineDynamicStateCreateInfo* GetStateCreateInfo() noexcept
			{
				return &m_DynamicStateCreateI;
			}

		private:
			std::vector<VkDynamicState> m_DynamicStates;
			VkPipelineDynamicStateCreateInfo m_DynamicStateCreateI;
		};

		class RasterizerConfig
		{
		public:
			RasterizerConfig(const RasterizerConfig& Other) = delete;
			RasterizerConfig(RasterizerConfig&& Other) = delete;

			RasterizerConfig();
			~RasterizerConfig() = default;

			VkPipelineRasterizationStateCreateInfo* GetStateCreateInfo() noexcept
			{
				return &m_RasterizerStateCreateI;
			}

		private:
			VkPipelineRasterizationStateCreateInfo m_RasterizerStateCreateI;
		};

		class MultisampleConfig
		{
		public:
			MultisampleConfig(const MultisampleConfig& Other) = delete;
			MultisampleConfig(MultisampleConfig&& Other) = delete;

			MultisampleConfig();
			~MultisampleConfig() = default;

			VkPipelineMultisampleStateCreateInfo* GetStateCreateInfo() noexcept
			{
				return &m_MSCreateI;
			}

		private:
			VkPipelineMultisampleStateCreateInfo m_MSCreateI;
		};

		class ColourBlendConfig
		{
		public:
			ColourBlendConfig(const ColourBlendConfig& Other) = delete;
			ColourBlendConfig(ColourBlendConfig&& Other) = delete;

			ColourBlendConfig();
			~ColourBlendConfig() = default;

			VkPipelineColorBlendStateCreateInfo* GetStateCreateInfo() noexcept
			{
				return &m_CBCreateI;
			}

		private:
			VkPipelineColorBlendStateCreateInfo m_CBCreateI;
			VkPipelineColorBlendAttachmentState m_CBAState;
		};

		class ViewportConfig
		{
		public:
			ViewportConfig(const ViewportConfig& Other) = delete;
			ViewportConfig(ViewportConfig&& Other) = delete;

			ViewportConfig();
			~ViewportConfig() = default;

			void SetViewportAndScissors(VkExtent2D& Extent) noexcept;

			VkViewport* GetViewportH() noexcept { return &m_Viewport; }
			VkRect2D* GetScissorsH() noexcept { return &m_Scissors; }
			VkPipelineViewportStateCreateInfo* GetStateCreateInfo() noexcept
			{
				return &m_VPStateCreateI;
			}

		private:
			VkViewport						  m_Viewport;
			VkRect2D						  m_Scissors;
			VkPipelineViewportStateCreateInfo m_VPStateCreateI;
		};
	}
}

