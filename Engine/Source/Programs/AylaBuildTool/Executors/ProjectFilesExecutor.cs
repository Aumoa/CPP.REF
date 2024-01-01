// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.CompilerServices;
using AE.Exceptions;
using AE.Platform.Windows;
using AE.ProjectFiles.VisualStudio;
using AE.Projects;
using AE.System;

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
        Workspace workspace = await ConfigureWorkspaceAsync(cancellationToken);
        VisualStudioSolution solution = new(workspace, ProjectFile);
        var toolChain = VisualStudioInstallation.FindVisualStudioInstallations(CompilerVersion.VisualStudio2022).FirstOrDefault();
        if (toolChain == null)
        {
            throw new TerminateException(KnownErrorCode.NotSupportedCompiler, CoreStrings.Errors.CompilerNotSupported);
        }

        ToolChain.Init(toolChain);
        Target.Create(VisualStudioSolution.Rules, workspace.CurrentTarget);
        workspace.SearchModules();

        await solution.ConfigureRulesAsync(cancellationToken);
        await solution.GenerateProjectFilesAsync(cancellationToken);
        return 0;
    }
}
