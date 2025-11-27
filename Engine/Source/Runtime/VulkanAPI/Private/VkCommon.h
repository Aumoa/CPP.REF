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

    template<class TSelf>
    inline auto Get(this TSelf&& self) noexcept
    {
        return self.m_Ptr;
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

    template<class TSelf>
    inline auto operator ->(this TSelf&& self) noexcept { return self.Get(); }

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

#undef __ALLOW_PLATFORM_COMMON_H__
