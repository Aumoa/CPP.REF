// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public class ClassNotFoundException : Exception
{
    public ClassNotFoundException(string assemblyName, Type classType)
        : base(FormatMessage(assemblyName, classType))
    {
    }

    private static string FormatMessage(string assemblyName, Type classType)
    {
        return $"Cannot find `{classType.Name}` class in `{assemblyName}` assembly.";
    }
}
