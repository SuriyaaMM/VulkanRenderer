
#pragma once

#include "Standard.h"
#include "Vulkan.h"

#define MAX_ASYNC_FRAMES	2

#if defined(_DEBUG)

#define CHECK(__VKRESULT__, __MESSAGE__) if(__VKRESULT__ != VK_SUCCESS){\
	std::cout << "[Renderer]: "<<__MESSAGE__ << std::endl;\
	std::cout << "[Return Code]: " << string_VkResult(__VKRESULT__) << std::endl;\
	__debugbreak();}

#define VALIDATE_HANDLE(__POINTER__) if(__POINTER__ == nullptr){\
	std::cout << "[Renderer]: Pointer at \n Line: " << __LINE__ << "\n File: " << __FILE__\
	<< "\n was found to be nullptr!" << std::endl;\
	__debugbreak();}

#else

#define CHECK(__VKRESULT__, __MESSAGE__)
#define VALIDATE_HANDLE(__POINTER__)

#endif

