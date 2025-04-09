// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal class VSUtility
{
    public static string GetArchitectureName(TargetInfo value) => value.Platform.Architecture switch
    {
        Architecture.X64 => "x64",
        _ => throw new InvalidOperationException()
    };

    public static string GetConfigName(TargetInfo value)
    {
        return value.Config.ToString() + (value.Editor ? " Editor" : string.Empty);
    }

    public static string GetCppConfigName(TargetInfo value)
    {
        return value.Config.ToString() + (value.Editor ? "_Editor" : string.Empty);
    }
}
