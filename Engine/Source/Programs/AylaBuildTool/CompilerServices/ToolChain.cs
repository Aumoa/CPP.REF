// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Platform;
using AE.Platform.Linux;
using AE.Rules;
using AE.System;

namespace AE.CompilerServices;

public static class ToolChain
{
    private static ToolChainInstallation? s_Current;

    public static ToolChainInstallation Current => s_Current ?? throw new InvalidOperationException("Init function is not called.");

    public static void Clear()
    {
        s_Current = null;
    }

    public static void Init(ToolChainInstallation toolChain)
    {
        s_Current = toolChain;
    }

    public static string GetShaderCompilerDirectory(Architecture targetArchitecture) => Current.GetShaderCompilerDirectory(targetArchitecture);

    public static Compiler SpawnCompiler() => Current.SpawnCompiler();

    public static Linker SpawnLinker() => Current.SpawnLinker();

    public static string DotNET => Current.DotNET;

    public static string ToolChainSignature => Current.GetToolChainSignature();
}
