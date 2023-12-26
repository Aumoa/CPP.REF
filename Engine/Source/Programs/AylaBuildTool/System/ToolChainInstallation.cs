// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Misc;
using AE.Platform;
using AE.Platform.Windows;

namespace AE.System;

public abstract class ToolChainInstallation
{
    public abstract string BaseDirectory { get; }

    public abstract Version InstallVersion { get; }

    public override string ToString()
    {
        return $"{InstallVersion} in {BaseDirectory}";
    }

    public abstract string[] GetRequiredExecutablePaths(Architecture TargetArchitecture);

    public abstract string[] GetRequiredLibraryPaths(Architecture TargetArchitecture);

    public abstract string[] GetRequiredIncludePaths(Architecture TargetArchitecture);

    public abstract Compiler SpawnCompiler();

    public abstract Linker SpawnLinker();

    public abstract string GetOutputExtension();

    public abstract string GetShaderCompilerDirectory(Architecture TargetArchitecture);

    public virtual string GetToolChainSignature()
    {
        ulong hashCode = 0;
        foreach (var architecture in new[] { Architecture.x64 })
        {
            hashCode ^= GetToolChainSignatureHashForArchitecture(architecture);
        }
        return $"{nameof(VisualStudioInstallation)}-{hashCode}";
    }

    private ulong GetToolChainSignatureHashForArchitecture(Architecture targetArchitecture)
    {
        var executablePaths = GetRequiredExecutablePaths(targetArchitecture);
        var shaderCompilerPath = GetShaderCompilerDirectory(targetArchitecture);
        var includePaths = GetRequiredIncludePaths(targetArchitecture);
        var libraryPaths = GetRequiredLibraryPaths(targetArchitecture);

        ulong hash = 0;
        foreach (var executablePath in executablePaths)
        {
            hash ^= CRC64.Generate64(executablePath);
        }
        hash ^= CRC64.Generate64(shaderCompilerPath);
        foreach (var includePath in includePaths)
        {
            hash ^= CRC64.Generate64(includePath);
        }
        foreach (var libraryPath in libraryPaths)
        {
            hash ^= CRC64.Generate64(libraryPath);
        }
        return hash;
    }

    public abstract string DotNET { get; }
}
