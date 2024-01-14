// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.CompilerServices;
using AE.Exceptions;
using AE.Platform.Windows;
using AE.ProjectFiles.VisualStudio;
using AE.Projects;

namespace AE.Executors;

public class ProjectFilesExecutor : ProjectBasedExecutor, IExecutor
{
    private record Arguments
    {
    }

    private readonly Arguments GeneratorArgs = new();

    public ProjectFilesExecutor(CommandLineParser Args) : base(Args)
    {
        Args.ApplyTo(GeneratorArgs);
    }

    public async Task<int> RunAsync(CancellationToken cancellationToken = default)
    {
        ConfigureWorkspace();
        await Workspace.InitAssembliesAsync(cancellationToken);

        var toolChain = VisualStudioInstallation.FindVisualStudioInstallations(CompilerVersion.VisualStudio2022).FirstOrDefault();
        if (toolChain == null)
        {
            throw new TerminateException(KnownErrorCode.NotSupportedCompiler, CoreStrings.Errors.CompilerNotSupported);
        }

        ToolChain.Init(toolChain);
        Target.Create(VisualStudioSolution.TargetInfo, true);

        foreach (var innerCppModule in Workspace.CppAssemblies)
        {
            ModuleDependencyCache.BuildCache(innerCppModule.Name, string.Empty);
        }

        await VisualStudioSolution.GenerateProjectFilesAsync(cancellationToken);
        return 0;
    }
}
