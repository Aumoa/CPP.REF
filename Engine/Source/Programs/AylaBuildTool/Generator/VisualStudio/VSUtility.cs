// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Runtime.InteropServices;

namespace AylaEngine;

internal class VSUtility
{
    public static string GetArchitectureName(TargetInfo value) => GetArchitectureName(value.Platform.Architecture);

    public static string GetArchitectureName(Architecture value) => value switch
    {
        Architecture.X64 => "x64",
        Architecture.Arm64 => "arm64",
        _ => throw new InvalidOperationException()
    };

    public static string RemapPlatformName(TargetInfo value) => value.Platform.Name == "Win64" ? "x64" : value.Platform.Name;

    public static string GetConfigName(TargetInfo value)
    {
        return value.Config.ToString() + (value.Editor ? " Editor" : string.Empty);
    }

    public static string GetCppConfigName(TargetInfo value)
    {
        return value.Config.ToString() + (value.Editor ? "_Editor" : string.Empty);
    }
}
