// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include <X11/Xlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#include <vulkan/vk_enum_string_helper.h>

#define VKR(expr) \
if (auto VKR_res__ = (expr); VKR_res__ != VK_SUCCESS) \
{ \
    throw ::Ayla::InvalidOperationException(String::FromLiteral(string_VkResult(VKR_res__))); \
}
