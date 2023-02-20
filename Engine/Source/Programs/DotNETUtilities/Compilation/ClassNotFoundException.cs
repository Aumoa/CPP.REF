// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Compilation;

public class ClassNotFoundException : Exception
{
    public ClassNotFoundException(string AssemblyName, Type ClassType)
        : base(FormatMessage(AssemblyName, ClassType))
    {
    }

    private static string FormatMessage(string AssemblyName, Type ClassType)
    {
        return $"Cannot find `{ClassType.Name}` class in `{AssemblyName}` assembly.";
    }
}
