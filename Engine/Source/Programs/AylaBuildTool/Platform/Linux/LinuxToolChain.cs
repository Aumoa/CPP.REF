// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.Exceptions;
using AE.System;

namespace AE.Platform.Linux;

public class LinuxToolChain : ToolChainInstallation
{
    private readonly string CompilerPath;

    public LinuxToolChain()
    {
        CompilerPath = SearchSymbolicRootRecursive("/usr/bin/gcc")!;
        BaseDirectory = Path.GetDirectoryName(CompilerPath) ?? "/usr/bin";

        ProcessStartInfo PSI = new(CompilerPath);
        PSI.WorkingDirectory = BaseDirectory;
        PSI.Arguments = "-v";
        PSI.RedirectStandardOutput = true;
        PSI.RedirectStandardError = true;
        PSI.UseShellExecute = false;
        PSI.CreateNoWindow = true;

        Process Gcc = Process.Start(PSI)!;
        string Output = Gcc.StandardError.ReadToEnd();
        Gcc.WaitForExit();
        Output += Gcc.StandardError.ReadToEnd();

        string Last = Output.Split('\n', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries)[^1];
        string VersionStr = Last.Substring("gcc version ".Length).Split(' ', StringSplitOptions.TrimEntries)[0];
        InstallVersion = Version.Parse(VersionStr);
    }

    private static string SearchSymbolicRootRecursive(string InPath)
    {
        while (true)
        {
            var Info = new FileInfo(InPath);
            if (Info.Exists == false)
            {
                return InPath;
            }

            if (File.Exists(Info.LinkTarget) == false)
            {
                return InPath;
            }

            InPath = Info.LinkTarget;
        }
    }

    public override string BaseDirectory { get; }

    public override Version InstallVersion { get; }

    public bool bIsPreview { get; init; } = false;

    public override string[] GetRequiredExecutablePaths(Architecture TargetArchitecture)
    {
        return Array.Empty<string>();
    }

    public override string[] GetRequiredLibraryPaths(Architecture TargetArchitecture)
    {
        return Array.Empty<string>();
    }

    public override string[] GetRequiredIncludePaths(Architecture TargetArchitecture)
    {
        return Array.Empty<string>();
    }

    public override Compiler SpawnCompiler()
    {
        return new GccCompiler(this, CompilerPath);
    }

    public override Linker SpawnLinker()
    {
        return new GccLinker(this, CompilerPath);
    }

    public override string GetOutputExtension()
    {
        return ".o";
    }

    public override string GetShaderCompilerDirectory(Architecture TargetArchitecture)
    {
        return BaseDirectory;
    }
}
