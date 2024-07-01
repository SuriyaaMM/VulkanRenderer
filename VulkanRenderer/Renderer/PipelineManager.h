
#pragma once

#include "Shaders.h"
#include "SwapchainManager.h"
#include "PipelineStateConfigurators.h"

namespace Fox
{
	namespace vk
	{
		class PipelineManager : public DLManager
		{
		public:
			PipelineManager(const PipelineManager& Source) = delete;
			PipelineManager(PipelineManager&& Source) = delete;

			PipelineManager(DeviceManager* pDeviceManager, VkSurfaceFormatKHR& SurfaceFormat);
			~PipelineManager() = default;

			void SetViewportAndScissor(VkExtent2D& Extent) noexcept;

			virtual void DestroyResources() noexcept override;

			VkRenderPass*			GetRenderPassH()		noexcept { return &m_RenderPass; }
			VkPipelineLayout&		GetPipelineLayout()		noexcept { return m_PipelineLayout; }
			VkPipeline*				GetPipelineH()			noexcept { return &m_GraphicsPipeline; }
			VkViewport&				GetViewport()			noexcept { return m_Viewport; }
			VkRect2D&				GetScissor()			noexcept { return m_Scissors; }
			//VkDescriptorSetLayout&	GetUniformDescriptor()	noexcept { return m_UniformLayout; }

			ShaderManager& GetShaderManager()		noexcept { return m_ShaderManager; }

			std::vector<VkDynamicState>& GetDynamicStatesV() noexcept { return m_DynamicStates; }

		private:
			Config::VertexInputConfig			m_VIConfig;
			Config::InputAssemblyConfig			m_IAConfig;

			ShaderManager						m_ShaderManager;

			std::vector<VkDynamicState>			m_DynamicStates;

			//VkDescriptorSetLayout				m_UniformLayout;
			VkPipelineLayout					m_PipelineLayout;
			VkRenderPass						m_RenderPass;
			VkPipeline							m_GraphicsPipeline;
			VkViewport							m_Viewport;
			VkRect2D							m_Scissors;
		};
	}
}

