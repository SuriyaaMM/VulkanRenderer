#include "Shaders.h"

namespace Fox
{
	namespace Resource
	{
		Shader::Shader(vk::DeviceManager* pDeviceManager, VkShaderStageFlagBits ShaderStage,
			std::string&& SpirvFilePath)
			:
			MResource(pDeviceManager->GetDeviceH()),
			m_ShaderCreateI(), m_ShaderModule(VK_NULL_HANDLE)
		{
			std::fstream FileHandle = std::fstream(SpirvFilePath,
				std::ios::beg | std::ios::binary | std::ios::in);

			if (FileHandle.is_open() && FileHandle.good())
			{
				std::string FileContent = std::string(std::istreambuf_iterator<char>(FileHandle),
					std::istreambuf_iterator<char>());

				VkShaderModuleCreateInfo ShaderModuleI = {};
				ShaderModuleI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				ShaderModuleI.codeSize = FileContent.size();
				ShaderModuleI.pCode = reinterpret_cast<const uint32_t*>(FileContent.c_str());
				ShaderModuleI.flags = 0;
				ShaderModuleI.pNext = nullptr;

				Debug::Result = vkCreateShaderModule(*pDevice, &ShaderModuleI, nullptr, &m_ShaderModule);
				CHECK(Debug::Result, "Vertex Shader Creation Failed!");

				m_ShaderCreateI.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				m_ShaderCreateI.stage = ShaderStage;
				m_ShaderCreateI.module = m_ShaderModule;
				m_ShaderCreateI.pName = "main";
				m_ShaderCreateI.flags = 0;
				m_ShaderCreateI.pSpecializationInfo = nullptr;
				m_ShaderCreateI.pNext = nullptr;
			}
			else
			{
				std::cout << "[Renderer]: Opening of Vertex Shader File Failed!" << std::endl;
				std::cout << "Reason: " << strerror(errno);
				std::terminate();
			}
		}

		void Shader::DestroyResource() noexcept
		{
			VALIDATE_HANDLE(pDevice);
			VALIDATE_HANDLE(m_ShaderModule);
			vkDestroyShaderModule(*pDevice, m_ShaderModule, nullptr);
		}
	}

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