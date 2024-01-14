// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Xml.Linq;

using AE.BuildSettings;
using AE.CLI;
using AE.CompilerServices;
using AE.Exceptions;
using AE.Platform;
using AE.Projects;
using AE.Rules;
using AE.System;

namespace AE.Executors;

public class BuildExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
        [CommandLineApply(CategoryName = "Target", IsRequired = true)]
        public string Target { get; set; } = null!;

        [CommandLineApply(CategoryName = "Clean")]
        public bool Clean { get; set; }

        [CommandLineApply(CategoryName = "Config")]
        public string Config { get; set; } = null!;

        [CommandLineApply(CategoryName = "Platform")]
        public string? Platform { get; set; } = null!;
    }

    private readonly Arguments buildArgs = new();
    private readonly Configuration projectConfig;

    public BuildExecutor(CommandLineParser args) : base(args)
    {
        args.ApplyTo(buildArgs);
        buildArgs.Config ??= "Shipping";
        projectConfig = Enum.Parse<Configuration>(buildArgs.Config);
    }

    public async Task<int> RunAsync(CancellationToken cancellationToken = default)
    {
        ConfigureWorkspace();
        if (buildArgs.Clean == true)
        {
            Workspace.Cleanup();
        }

        await Workspace.InitAssembliesAsync(cancellationToken);

        var hostPlatform = Environment.OSVersion.Platform switch
        {
            PlatformID.Win32NT => TargetPlatform.Win64,
            PlatformID.Unix => TargetPlatform.Linux,
            _ => throw new TerminateException(KnownErrorCode.NotSupportedBuildHostPlatform, CoreStrings.Errors.NotSupportedBuildHostPlatform)
        };

        var targetInfo = new TargetInfo
        {
            BuildConfiguration = new()
            {
                Configuration = projectConfig,
                Platform = buildArgs.Platform != null ? TargetPlatform.FromTargetName(buildArgs.Platform) : hostPlatform
            }
        };

        Target.Create(targetInfo, true);
        ModuleDependencyCache.BuildCache(buildArgs.Target, string.Empty);

        if (ModuleDependencyCache.GetCached(buildArgs.Target, string.Empty).DependModules.Contains("RHI"))
        {
            ModuleDependencyCache.Push();
            Target.Push();

            var binariesTargetInfo = new TargetInfo
            {
                BuildConfiguration = new()
                {
                    Configuration = Configuration.Shipping,
                    Platform = hostPlatform
                }
            };

            Target.Create(binariesTargetInfo, false);
            ModuleDependencyCache.BuildCache("ShaderCompileWorker", string.Empty);

            if (await ModuleCompiler.CompileAsync("ShaderCompileWorker", Global.EngineDirectory, true, cancellationToken) == false)
            {
                return -1;
            }

            Target.Pop();
            ModuleDependencyCache.Pop();
        }


        if (await ModuleCompiler.CompileAsync(buildArgs.Target, Workspace.Current, cancellationToken: cancellationToken) == false)
        {
            return -1;
        }

        return 0;
    }
}
