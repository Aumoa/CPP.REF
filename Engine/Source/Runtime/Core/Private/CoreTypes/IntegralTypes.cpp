// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreTypes/IntegralTypes.h"
#include "CoreTypes/String.h"

template<>
String TIntegralType<int8>::ToString() const
{
    return String(std::to_wstring(Value));
}

template<>
String TIntegralType<uint8>::ToString() const
{
    return String(std::to_wstring(Value));
}

template<>
String TIntegralType<int16>::ToString() const
{
    return String(std::to_wstring(Value));
}

template<>
String TIntegralType<uint16>::ToString() const
{
    return String(std::to_wstring(Value));
}

template<>
String TIntegralType<int32>::ToString() const
{
    return String(std::to_wstring(Value));
}

template<>
String TIntegralType<uint32>::ToString() const
{
    return String(std::to_wstring(Value));
}

template<>
String TIntegralType<int64>::ToString() const
{
    return String(std::to_wstring(Value));
}

template<>
String TIntegralType<uint64>::ToString() const
{
    return String(std::to_wstring(Value));
}