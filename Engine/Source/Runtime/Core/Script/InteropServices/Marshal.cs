// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace Ayla;

public static class Marshal
{
    public static nint GetInstancePointer(Object target)
    {
        return target.m_InstancePtr;
    }
}
