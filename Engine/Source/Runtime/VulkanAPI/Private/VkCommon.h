// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <vulkan/vulkan.h>
#include "LogVulkan.h"
#if PLATFORM_LINUX
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>
#elif PLATFORM_WINDOWS
#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"
#include <vulkan/vulkan_win32.h>
#endif
#include <vulkan/vk_enum_string_helper.h>
#include <format>

inline void VKR__impl(Ayla::String s)
{
    Ayla::LogVulkan::Critical(s);
    throw Ayla::InvalidOperationException(s);
}

#define VKR(expr, ...) \
if (auto VKR_res__ = (expr); VkrFailure __VA_OPT__(<) __VA_ARGS__ __VA_OPT__(>) (VKR_res__)) \
{ \
    VKR__impl(Ayla::String::Format(TEXT("{}"), VKR_res__)); \
}

#define DECLARE_FORMATTER(T) \
template<> \
struct std::formatter<T, wchar_t> : public std::formatter<::Ayla::String, wchar_t> \
{ \
    template<class TFormatContext> \
    auto format(const T& value, TFormatContext& context) const \
    { \
        return std::formatter<::Ayla::String, wchar_t>::format(::Ayla::String::FromLiteral(string_ ## T(value)), context); \
    } \
};

DECLARE_FORMATTER(VkResult);
DECLARE_FORMATTER(VkFormat);

template<VkResult... Excepts_>
constexpr bool VkrFailure(VkResult result) noexcept
{
    if constexpr (sizeof...(Excepts_) == 0)
    {
        return result != VK_SUCCESS;
	}
    else
    {
	    return result != VK_SUCCESS && ((result != Excepts_) && ...);
    }
}

template<class T, void(*DestroyFunction)(T, const VkAllocationCallbacks*)>
class VkRef
{
    VkRef(const VkRef&) = delete;
    VkRef& operator =(const VkRef&) = delete;

private:
    T m_Ptr = nullptr;
    
public:
    inline VkRef() noexcept
    {
    }

    inline VkRef(const VkRef&& rhs) noexcept
        : m_Ptr{ rhs.m_Ptr }
    {
        rhs.m_Ptr = nullptr;
    }
    
    inline ~VkRef() noexcept
    {
        this->Reset();
    }

    inline T Get() noexcept
    {
        return m_Ptr;
    }

    inline T Get() const noexcept
    {
        return m_Ptr;
    }

    inline void Reset() noexcept
    {
        if (m_Ptr != nullptr)
        {
            DestroyFunction(m_Ptr, nullptr);
            m_Ptr = nullptr;
        }
    }

    inline T* ReleaseAndGetAddressOf() noexcept
    {
        Reset();
        return &m_Ptr;
    }

    inline T operator ->() noexcept { return Get(); }
    inline T operator ->() const noexcept { return Get(); }

    inline VkRef& operator =(VkRef&& rhs) noexcept
    {
        Reset();
        m_Ptr = rhs.m_Ptr;
        rhs.m_Ptr = nullptr;
        return *this;
    }

    inline T* operator &() noexcept
    {
        return ReleaseAndGetAddressOf();
    }

    inline operator T() const noexcept
    {
        return Get();
    }
};

using VkInstanceRef = VkRef<VkInstance, vkDestroyInstance>;
using VkDeviceRef = VkRef<VkDevice, vkDestroyDevice>;

#include "Rendering/GraphicsFormat.h"
#include "Rendering/InputElementSemantic.h"

inline VkFormat ToVkFormat(Ayla::GraphicsFormat format)
{
	using enum Ayla::GraphicsFormat;
	switch (format)
	{
	case R32G32B32A32_Float:   return VK_FORMAT_R32G32B32A32_SFLOAT;
	case R32G32B32A32_UInt:    return VK_FORMAT_R32G32B32A32_UINT;
	case R32G32B32A32_SInt:    return VK_FORMAT_R32G32B32A32_SINT;
	case R32G32B32_Float:      return VK_FORMAT_R32G32B32_SFLOAT;
	case R32G32B32_UInt:       return VK_FORMAT_R32G32B32_UINT;
	case R32G32B32_SInt:       return VK_FORMAT_R32G32B32_SINT;
	case R16G16B16A16_Float:   return VK_FORMAT_R16G16B16A16_SFLOAT;
	case R16G16B16A16_UNorm:   return VK_FORMAT_R16G16B16A16_UNORM;
	case R16G16B16A16_UInt:    return VK_FORMAT_R16G16B16A16_UINT;
	case R16G16B16A16_SNorm:   return VK_FORMAT_R16G16B16A16_SNORM;
	case R16G16B16A16_SInt:    return VK_FORMAT_R16G16B16A16_SINT;
	case R32G32_Float:         return VK_FORMAT_R32G32_SFLOAT;
	case R32G32_UInt:          return VK_FORMAT_R32G32_UINT;
	case R32G32_SInt:          return VK_FORMAT_R32G32_SINT;
	case R8G8B8A8_UNorm:       return VK_FORMAT_R8G8B8A8_UNORM;
	case R8G8B8A8_UNorm_SRGB:  return VK_FORMAT_R8G8B8A8_SRGB;
	case R8G8B8A8_UInt:        return VK_FORMAT_R8G8B8A8_UINT;
	case R8G8B8A8_SNorm:       return VK_FORMAT_R8G8B8A8_SNORM;
	case R8G8B8A8_SInt:        return VK_FORMAT_R8G8B8A8_SINT;
	case R16G16_Float:         return VK_FORMAT_R16G16_SFLOAT;
	case R16G16_UNorm:         return VK_FORMAT_R16G16_UNORM;
	case R16G16_UInt:          return VK_FORMAT_R16G16_UINT;
	case R16G16_SNorm:         return VK_FORMAT_R16G16_SNORM;
	case R16G16_SInt:          return VK_FORMAT_R16G16_SINT;
	case D32_Float:            return VK_FORMAT_D32_SFLOAT;
	case R32_Float:            return VK_FORMAT_R32_SFLOAT;
	case R32_UInt:             return VK_FORMAT_R32_UINT;
	case R32_SInt:             return VK_FORMAT_R32_SINT;
	case D24_UNorm_S8_UInt:    return VK_FORMAT_D24_UNORM_S8_UINT;
	case R8G8_UNorm:           return VK_FORMAT_R8G8_UNORM;
	case R8G8_UInt:            return VK_FORMAT_R8G8_UINT;
	case R8G8_SNorm:           return VK_FORMAT_R8G8_SNORM;
	case R8G8_SInt:            return VK_FORMAT_R8G8_SINT;
	case R16_Float:            return VK_FORMAT_R16_SFLOAT;
	case D16_UNorm:            return VK_FORMAT_D16_UNORM;
	case R16_UNorm:            return VK_FORMAT_R16_UNORM;
	case R16_UInt:             return VK_FORMAT_R16_UINT;
	case R16_SNorm:            return VK_FORMAT_R16_SNORM;
	case R16_SInt:             return VK_FORMAT_R16_SINT;
	case R8_UNorm:             return VK_FORMAT_R8_UNORM;
	case R8_UInt:              return VK_FORMAT_R8_UINT;
	case R8_SNorm:             return VK_FORMAT_R8_SNORM;
	case R8_SInt:              return VK_FORMAT_R8_SINT;
	case B8G8R8A8_UNorm:       return VK_FORMAT_B8G8R8A8_UNORM;
	case B8G8R8A8_UNorm_SRGB:  return VK_FORMAT_B8G8R8A8_SRGB;
	default:                   return VK_FORMAT_UNDEFINED;
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__
