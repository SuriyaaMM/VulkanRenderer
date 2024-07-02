
#pragma once

#include "ShaderManager.h"
#include "SwapchainManager.h"
#include "PipelineStateConfigurators.h"

namespace Fox
{
	namespace Resource
	{
		class RenderPass : public CResource
		{
		public:
			RenderPass(const RenderPass& Other) = default;
			RenderPass(RenderPass&& Other) = default;
			RenderPass& operator=(const RenderPass& Other) = default;

			RenderPass(VkDevice* pValidDevice, VkSurfaceFormatKHR& Format);
			~RenderPass() = default;

			void DestroyResource() noexcept;
			
			VkRenderPass* GetRenderPassH() noexcept { return &m_RenderPass; }

		private:
			VkRenderPass m_RenderPass;
		};
	}

	namespace vk
	{
		class PipelineManager : public DLManager
		{
		public:
			PipelineManager(const PipelineManager& Source) = delete;
			PipelineManager(PipelineManager&& Source) = delete;

			PipelineManager(DeviceManager* pDeviceManager, VkSurfaceFormatKHR& SurfaceFormat);
			~PipelineManager() = default;

			void ReconstructGraphicsPipeline(VkSurfaceFormatKHR& SurfaceFormat);

			virtual void DestroyResources() noexcept override;

			VkPipelineLayout&		GetPipelineLayout()		noexcept { return m_PipelineLayout; }
			VkPipeline*				GetGraphicsPipelineH()	noexcept { return &m_GraphicsPipeline; }

			//VkDescriptorSetLayout&	GetUniformDescriptor()	noexcept { return m_UniformLayout; }

			ShaderManager& GetShaderManager()		noexcept { return m_ShaderManager; }
			Resource::RenderPass* GetRenderPassH()	noexcept { return &m_RenderPass; }

			Config::ViewportConfig& GetViewportConfig() noexcept { return m_VPConfig; }

		private:
			Config::VertexInputConfig			m_VIConfig;
			Config::InputAssemblyConfig			m_IAConfig;
			Config::DynamicStatesConfig			m_DynamicStatesConfig;
			Config::RasterizerConfig			m_RasterizerStateConfig;
			Config::MultisampleConfig			m_MSConfig;
			Config::ColourBlendConfig			m_CBConfig;
			Config::ViewportConfig				m_VPConfig;

			ShaderManager						m_ShaderManager;
			Resource::RenderPass				m_RenderPass;

			std::array<VkPipelineShaderStageCreateInfo, 2> m_ShaderStages;

			//VkDescriptorSetLayout				m_UniformLayout;
			VkPipelineLayout					m_PipelineLayout;
			VkPipeline							m_GraphicsPipeline;
		};
	}
}

