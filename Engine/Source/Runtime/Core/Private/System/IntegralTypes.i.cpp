// Copyright 2020-2024 Aumoa.lib. All right reserved.

#include <errno.h>
#include "Platform/PlatformMacros.h"

import Core;

static inline const wchar_t* GetNumberString(String str) noexcept
{
    static thread_local wchar_t s_CachedStrBuf[256];
    if (str[str.length()] == '\0')
    {
        return str.c_str();
    }
    else
    {
        memcpy(s_CachedStrBuf, str.c_str(), sizeof(wchar_t) * str.length());
        s_CachedStrBuf[str.length()] = 0;
        return s_CachedStrBuf;
    }
}

template<class TDest, class TInput>
FORCEINLINE bool VerifyRange(TInput inputValue, int32 err) noexcept
{
    if (err == EINVAL || err == ERANGE || std::numeric_limits<TDest>::lowest() > inputValue || std::numeric_limits<TDest>::max() < inputValue)
    {
        return false;
    }
    return true;
}

template<>
String TIntegralType<int8>::ToString(const UnderlyingType& value)
{
    return String(std::to_wstring(value));
}

template<>
bool TIntegralType<int8>::TryParse(String str, int8& outValue, int32 base) noexcept
{
    int32& err = errno = 0;
    const wchar_t* basePtr = GetNumberString(str);
    wchar_t* endPtr;
    int32 val = wcstol(basePtr, &endPtr, base);
    if (basePtr == endPtr || VerifyRange<int8>(val, err) == false)
    {
        outValue = 0;
        return false;
    }
    outValue = static_cast<int8>(val);
    return true;
}

template<>
String TIntegralType<uint8>::ToString(const UnderlyingType& value)
{
    return String(std::to_wstring(value));
}

template<>
bool TIntegralType<uint8>::TryParse(String str, uint8& outValue, int32 base) noexcept
{
    int32& err = errno = 0;
    const wchar_t* basePtr = GetNumberString(str);
    wchar_t* endPtr;
    uint32 val = wcstoul(basePtr, &endPtr, base);
    if (basePtr == endPtr || VerifyRange<uint8>(val, err) == false)
    {
        outValue = 0;
        return false;
    }
    outValue = static_cast<uint8>(val);
    return true;
}

template<>
String TIntegralType<int16>::ToString(const UnderlyingType& value)
{
    return String(std::to_wstring(value));
}

template<>
bool TIntegralType<int16>::TryParse(String str, int16& outValue, int32 base) noexcept
{
    int32& err = errno = 0;
    const wchar_t* basePtr = GetNumberString(str);
    wchar_t* endPtr;
    int32 val = wcstol(basePtr, &endPtr, base);
    if (basePtr == endPtr || VerifyRange<int16>(val, err) == false)
    {
        outValue = 0;
        return false;
    }
    outValue = static_cast<int16>(val);
    return true;
}

template<>
String TIntegralType<uint16>::ToString(const UnderlyingType& value)
{
    return String(std::to_wstring(value));
}

template<>
bool TIntegralType<uint16>::TryParse(String str, uint16& outValue, int32 base) noexcept
{
    int32& err = errno = 0;
    const wchar_t* basePtr = GetNumberString(str);
    wchar_t* endPtr;
    uint32 val = wcstoul(basePtr, &endPtr, base);
    if (basePtr == endPtr || VerifyRange<uint16>(val, err) == false)
    {
        outValue = 0;
        return false;
    }
    outValue = static_cast<uint16>(val);
    return true;
}

template<>
String TIntegralType<int32>::ToString(const UnderlyingType& value)
{
    return String(std::to_wstring(value));
}

template<>
bool TIntegralType<int32>::TryParse(String str, int32& outValue, int32 base) noexcept
{
    int32& err = errno = 0;
    const wchar_t* basePtr = GetNumberString(str);
    wchar_t* endPtr;
    int32 val = wcstol(basePtr, &endPtr, base);
    if (basePtr == endPtr || VerifyRange<int32>(val, err) == false)
    {
        outValue = 0;
        return false;
    }
    outValue = static_cast<int32>(val);
    return true;
}

template<>
String TIntegralType<uint32>::ToString(const UnderlyingType& value)
{
    return String(std::to_wstring(value));
}

template<>
bool TIntegralType<uint32>::TryParse(String str, uint32& outValue, int32 base) noexcept
{
    int32& err = errno = 0;
    const wchar_t* basePtr = GetNumberString(str);
    wchar_t* endPtr;
    uint32 val = wcstoul(basePtr, &endPtr, base);
    if (basePtr == endPtr || VerifyRange<uint32>(val, err) == false)
    {
        outValue = 0;
        return false;
    }
    outValue = static_cast<uint32>(val);
    return true;
}

template<>
String TIntegralType<int64>::ToString(const UnderlyingType& value)
{
    return String(std::to_wstring(value));
}

template<>
bool TIntegralType<int64>::TryParse(String str, int64& outValue, int32 base) noexcept
{
    int32& err = errno = 0;
    const wchar_t* basePtr = GetNumberString(str);
    wchar_t* endPtr;
    int64 val = wcstoll(basePtr, &endPtr, base);
    if (basePtr == endPtr || VerifyRange<int64>(val, err) == false)
    {
        outValue = 0;
        return false;
    }
    outValue = static_cast<int64>(val);
    return true;
}

template<>
String TIntegralType<uint64>::ToString(const UnderlyingType& value)
{
    return String(std::to_wstring(value));
}

template<>
bool TIntegralType<uint64>::TryParse(String str, uint64& outValue, int32 base) noexcept
{
    int32& err = errno = 0;
    const wchar_t* basePtr = GetNumberString(str);
    wchar_t* endPtr;
    uint64 val = wcstoull(basePtr, &endPtr, base);
    if (basePtr == endPtr || VerifyRange<uint64>(val, err) == false)
    {
        outValue = 0;
        return false;
    }
    outValue = static_cast<uint64>(val);
    return true;
}