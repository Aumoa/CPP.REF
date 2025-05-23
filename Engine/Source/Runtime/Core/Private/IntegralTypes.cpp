// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IntegralTypes.h"
#include "String_.h"

namespace Ayla
{
    static inline const wchar_t* GetNumberString(String InStr) noexcept
    {
        static thread_local wchar_t CachedStrBuf[256];
        if (InStr[InStr.length()] == '\0')
        {
            return InStr.c_str();
        }
        else
        {
            memcpy(CachedStrBuf, InStr.c_str(), sizeof(wchar_t) * InStr.length());
            CachedStrBuf[InStr.length()] = 0;
            return CachedStrBuf;
        }
    }

    template<class TDest, class TInput>
    FORCEINLINE bool VerifyRange(TInput InputValue, int32 Err) noexcept
    {
        if (Err == EINVAL || Err == ERANGE || std::numeric_limits<TDest>::lowest() > InputValue || std::numeric_limits<TDest>::max() < InputValue)
        {
            return false;
        }
        return true;
    }

    template<>
    String TIntegralType<int8>::ToString(const UnderlyingType& InValue)
    {
        return String(std::to_wstring(InValue));
    }

    template<>
    bool TIntegralType<int8>::TryParse(String InStr, int8& OutValue, int32 Base) noexcept
    {
        int32& Err = errno = 0;
        const wchar_t* StartPtr = GetNumberString(InStr);
        wchar_t* EndPtr;
        int32 Val = wcstol(StartPtr, &EndPtr, Base);
        if (StartPtr == EndPtr || VerifyRange<int8>(Val, Err) == false)
        {
            OutValue = 0;
            return false;
        }
        OutValue = static_cast<int8>(Val);
        return true;
    }

    template<>
    String TIntegralType<uint8>::ToString(const UnderlyingType& InValue)
    {
        return String(std::to_wstring(InValue));
    }

    template<>
    bool TIntegralType<uint8>::TryParse(String InStr, uint8& OutValue, int32 Base) noexcept
    {
        int32& Err = errno = 0;
        const wchar_t* StartPtr = GetNumberString(InStr);
        wchar_t* EndPtr;
        uint32 Val = wcstoul(StartPtr, &EndPtr, Base);
        if (StartPtr == EndPtr || VerifyRange<uint8>(Val, Err) == false)
        {
            OutValue = 0;
            return false;
        }
        OutValue = static_cast<uint8>(Val);
        return true;
    }

    template<>
    String TIntegralType<int16>::ToString(const UnderlyingType& InValue)
    {
        return String(std::to_wstring(InValue));
    }

    template<>
    bool TIntegralType<int16>::TryParse(String InStr, int16& OutValue, int32 Base) noexcept
    {
        int32& Err = errno = 0;
        const wchar_t* StartPtr = GetNumberString(InStr);
        wchar_t* EndPtr;
        int32 Val = wcstol(StartPtr, &EndPtr, Base);
        if (StartPtr == EndPtr || VerifyRange<int16>(Val, Err) == false)
        {
            OutValue = 0;
            return false;
        }
        OutValue = static_cast<int16>(Val);
        return true;
    }

    template<>
    String TIntegralType<uint16>::ToString(const UnderlyingType& InValue)
    {
        return String(std::to_wstring(InValue));
    }

    template<>
    bool TIntegralType<uint16>::TryParse(String InStr, uint16& OutValue, int32 Base) noexcept
    {
        int32& Err = errno = 0;
        const wchar_t* StartPtr = GetNumberString(InStr);
        wchar_t* EndPtr;
        uint32 Val = wcstoul(StartPtr, &EndPtr, Base);
        if (StartPtr == EndPtr || VerifyRange<uint16>(Val, Err) == false)
        {
            OutValue = 0;
            return false;
        }
        OutValue = static_cast<uint16>(Val);
        return true;
    }

    template<>
    String TIntegralType<int32>::ToString(const UnderlyingType& InValue)
    {
        return String(std::to_wstring(InValue));
    }

    template<>
    bool TIntegralType<int32>::TryParse(String InStr, int32& OutValue, int32 Base) noexcept
    {
        int32& Err = errno = 0;
        const wchar_t* StartPtr = GetNumberString(InStr);
        wchar_t* EndPtr;
        int32 Val = wcstol(StartPtr, &EndPtr, Base);
        if (StartPtr == EndPtr || VerifyRange<int32>(Val, Err) == false)
        {
            OutValue = 0;
            return false;
        }
        OutValue = static_cast<int32>(Val);
        return true;
    }

    template<>
    String TIntegralType<uint32>::ToString(const UnderlyingType& InValue)
    {
        return String(std::to_wstring(InValue));
    }

    template<>
    bool TIntegralType<uint32>::TryParse(String InStr, uint32& OutValue, int32 Base) noexcept
    {
        int32& Err = errno = 0;
        const wchar_t* StartPtr = GetNumberString(InStr);
        wchar_t* EndPtr;
        uint32 Val = wcstoul(StartPtr, &EndPtr, Base);
        if (StartPtr == EndPtr || VerifyRange<uint32>(Val, Err) == false)
        {
            OutValue = 0;
            return false;
        }
        OutValue = static_cast<uint32>(Val);
        return true;
    }

    template<>
    String TIntegralType<int64>::ToString(const UnderlyingType& InValue)
    {
        return String(std::to_wstring(InValue));
    }

    template<>
    bool TIntegralType<int64>::TryParse(String InStr, int64& OutValue, int32 Base) noexcept
    {
        int32& Err = errno = 0;
        const wchar_t* StartPtr = GetNumberString(InStr);
        wchar_t* EndPtr;
        int64 Val = wcstoll(StartPtr, &EndPtr, Base);
        if (StartPtr == EndPtr || VerifyRange<int64>(Val, Err) == false)
        {
            OutValue = 0;
            return false;
        }
        OutValue = static_cast<int64>(Val);
        return true;
    }

    template<>
    String TIntegralType<uint64>::ToString(const UnderlyingType& InValue)
    {
        return String(std::to_wstring(InValue));
    }

    template<>
    bool TIntegralType<uint64>::TryParse(String InStr, uint64& OutValue, int32 Base) noexcept
    {
        int32& Err = errno = 0;
        const wchar_t* StartPtr = GetNumberString(InStr);
        wchar_t* EndPtr;
        uint64 Val = wcstoull(StartPtr, &EndPtr, Base);
        if (StartPtr == EndPtr || VerifyRange<uint64>(Val, Err) == false)
        {
            OutValue = 0;
            return false;
        }
        OutValue = static_cast<uint64>(Val);
        return true;
    }
}