// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#undef interface

#include "WindowsMinimal.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "Logging/LogCategoryBase.h"
#include "VulkanException.h"

#ifndef interface
#define interface struct
#endif

extern LogCategoryBase LogVulkanRHI;

#define VK_FAILED(Result) (Result != VK_SUCCESS)
#define VK_SUCCEEDED(Result) (Result == VK_SUCCESS)
#define VKR(Expression) if (VkResult vkResult = Expression; VK_FAILED(vkResult)) throw SC::Runtime::Game::VulkanRHI::VulkanException(vkResult)