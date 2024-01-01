// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.BuildSettings;
using AE.CLI;
using AE.CompilerServices;
using AE.Exceptions;
using AE.Misc;
using AE.Platform;
using AE.Platform.Linux;
using AE.Platform.Windows;
using AE.Projects;
using AE.Rules;
using AE.SourceTree;
using AE.System;

namespace AE.Executors;

public class BuildExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "Target", IsRequired = true)]
        public string Target { get; set; } = null!;

        [CommandLineApply(CategoryName = "Clean")]
        public bool bClean { get; set; }

        [CommandLineApply(CategoryName = "Config")]
        public string Config { get; set; } = null!;

        [CommandLineApply(CategoryName = "Platform")]
        public string? Platform { get; set; } = null!;
    }

    private readonly Arguments BuildArgs = new();
    private readonly Configuration ProjectConfig;

    public BuildExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(BuildArgs);
        BuildArgs.Config ??= "Debug";
        ProjectConfig = Enum.Parse<Configuration>(BuildArgs.Config);
    }

    public async Task<int> RunAsync(CancellationToken SToken = default)
    {
        if (BuildArgs.bClean == true)
        {
            foreach (var Dir in GetProjectDirectories())
            {
                Dir.Cleanup();
            }
        }

        Workspace Workspace = await ConfigureWorkspaceAsync(SToken);

        TargetPlatform HostPlatform = Environment.OSVersion.Platform switch
        {
            PlatformID.Win32NT => TargetPlatform.Win64,
            PlatformID.Unix => TargetPlatform.Linux,
            _ => throw new TerminateException(KnownErrorCode.NotSupportedBuildHostPlatform, CoreStrings.Errors.NotSupportedBuildHostPlatform)
        };
        var ToolsTargetInfo = new TargetInfo
        {
            BuildConfiguration = new()
            {
                Configuration = Configuration.Shipping,
                Platform = HostPlatform
            }
        };

        ToolChain.Init(SpawnPlatformToolChain(ToolsTargetInfo));

        var HeaderToolCompiler = await AylaProjectCompiler.CreateCompilerAsync(Workspace, "AylaHeaderTool", ToolsTargetInfo, SToken);

        int ReturnCode = await HeaderToolCompiler.CompileAsync(SToken);
        if (ReturnCode != 0)
        {
            return ReturnCode;
        }

        var ShaderCompileWorkerCompiler = await AylaProjectCompiler.CreateCompilerAsync(Workspace, "ShaderCompileWorker", ToolsTargetInfo, SToken);

        ReturnCode = await ShaderCompileWorkerCompiler.CompileAsync(SToken);
        if (ReturnCode != 0)
        {
            return ReturnCode;
        }

        var ProjectCompiler = await AylaProjectCompiler.CreateCompilerAsync(Workspace, BuildArgs.Target, new TargetInfo
        {
            BuildConfiguration = new()
            {
                Configuration = ProjectConfig,
                Platform = BuildArgs.Platform != null ? TargetPlatform.FromTargetName(BuildArgs.Platform) : HostPlatform
            }
        });

        return await ProjectCompiler.CompileAsync(SToken);
    }

    private ToolChainInstallation SpawnPlatformToolChain(TargetInfo targetInfo)
    {
        if (Environment.OSVersion.Platform == PlatformID.Win32NT)
        {
            // Host:Windows Target:Windows
            if (targetInfo.BuildConfiguration.Platform == TargetPlatform.Win64)
            {
                var Installations = VisualStudioInstallation.FindVisualStudioInstallations(CompilerVersion.VisualStudio2022);
                if (Installations.Any())
                {
                    return Installations[0];
                }
            }
        }
        else if (Environment.OSVersion.Platform == PlatformID.Unix)
        {
            // Host:Linux Target:Linux
            if (targetInfo.BuildConfiguration.Platform == TargetPlatform.Linux)
            {
                return new LinuxToolChain();
            }
        }

        throw new TerminateException(KnownErrorCode.PlatformCompilerNotFound, CoreStrings.Errors.PlatformCompilerNotFound(targetInfo.BuildConfiguration.Platform.ToString()));
    }
}
