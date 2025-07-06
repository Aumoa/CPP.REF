// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include <X11/Xlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#include <vulkan/vk_enum_string_helper.h>

#define VKR(expr) \
if (auto VKR_res__ = (expr); VKR_res__ != VK_SUCCESS) \
{ \
    throw ::Ayla::InvalidOperationException(String::Format(TEXT("{}"), VKR_res__)); \
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
};

using VkInstanceRef = VkRef<VkInstance, vkDestroyInstance>;
