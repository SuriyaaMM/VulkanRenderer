
#pragma once

//GLFW Library Include
#include <GLFW/glfw3.h>

#if defined(_WIN32) || defined(_WIN64)

#define VK_USE_PLATFORM_WIN32_KHR

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#endif // defined(_WIN32) || defined(_WIN64)

#include <vma/vma.h>

//Vulkan Utility Includes
#include <vulkan/vk_enum_string_helper.h>

//GLM Include
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Fox
{
	namespace Debug
	{
		static VkResult Result = VK_SUCCESS;
	}

	struct GPUResource
	{
	public:
		GPUResource(const GPUResource& Other) = delete;
		GPUResource(GPUResource&& Other) = default;

		GPUResource(VmaAllocator* pValidAllocator)
			:pAllocator(pValidAllocator)
		{

		}

		~GPUResource() = default;

		virtual void DestroyResource() noexcept = 0;

	protected:
		VmaAllocator* pAllocator;
	};

	//Non Movable Resource
	struct NMResource
	{
	public:
		NMResource(const NMResource& Other) = delete;
		NMResource(NMResource& Other) = delete;
		NMResource(NMResource&& Other) = delete;

		NMResource(VkDevice* pToValidDevice)
			:pDevice(pToValidDevice)
		{

		}
		~NMResource() = default;

		virtual void DestroyResource() noexcept = 0;

	protected:
		VkDevice* pDevice;
	};

	//Movable Resource
	struct MResource
	{
	public:
		MResource(const MResource& Other) = delete;
		MResource(MResource&& Other) = default;

		MResource(VkDevice* pToValidDevice)
			:pDevice(pToValidDevice)
		{

		}
		~MResource() = default;

		virtual void DestroyResource() noexcept = 0;

	protected:
		VkDevice* pDevice;
	};

	/*
	* Base Manager
	*/
	struct Manager
	{
	public:

		Manager(const Manager& Other) = delete;
		Manager(Manager&& Other) = default;

		Manager() = default;
		~Manager() = default;

		virtual void DestroyResources() noexcept = 0;

	};

	/*
	* Instance Level Manager
	*/
	struct ILManager
	{
		ILManager(const ILManager& Other) = delete;
		ILManager(ILManager&& Other) = default;

		ILManager(VkInstance* pValidInstance)
			:pInstance(pValidInstance)
		{

		}
		~ILManager() = default;

		virtual void DestroyResources() noexcept = 0;

	protected:
		VkInstance* pInstance;
	};

	//Device Level Manager
	struct DLManager
	{
		DLManager(const DLManager& Other) = delete;
		DLManager(DLManager&& Other) = delete;

		DLManager(VkDevice* pValidDevice)
			:pDevice(pValidDevice)
		{

		}
		~DLManager() = default;

		virtual void DestroyResources() noexcept = 0;

	protected:
		VkDevice* pDevice;
	};

	//Base Mesh
	struct Mesh
	{
	public:
		Mesh(const Mesh& Other) = delete;
		Mesh(Mesh&& Other) = default;

		Mesh() = default;
		~Mesh() = default;

		virtual void Draw(void* pCommandBuffer) noexcept = 0;
		virtual void DestroyResources() noexcept = 0;
	};

	typedef glm::vec3 TPosition;
	typedef glm::vec3 TColour;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Colour;
	};

	struct Uniform
	{
		glm::mat4 Model;
		glm::mat4 View;
		glm::mat4 Projection;

	};

	namespace Utility
	{
		template<typename T>
		uint32_t SizeInBytes(std::vector<T>& Container) noexcept
		{
			return (uint32_t)(sizeof(Container[0]) * Container.size());
		}
	}
}

