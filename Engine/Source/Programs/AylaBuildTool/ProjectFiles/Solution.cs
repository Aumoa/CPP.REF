// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Projects;
using AE.Rules;
using AE.System;

namespace AE.ProjectFiles;

public abstract class Solution : ISolution
{
    protected Workspace workspace { get; }

    protected Solution(Workspace workspace)
    {
        this.workspace = workspace;
    }

    public abstract Task GenerateProjectFilesAsync(CancellationToken cancellationToken = default);

    public async Task ConfigureRulesAsync(CancellationToken cancellationToken = default)
    {
        List<Task> tasks = new();

        foreach (var module in workspace.GetCXXModules())
        {
            tasks.Add(module.ConfigureAsync(cancellationToken));
        }

        await Task.WhenAll(tasks);
    }

    private static TargetRules? s_Rules;

    public static TargetRules Rules
    {
        get
        {
            if (s_Rules == null)
            {
                var targetInfo = new TargetInfo()
                {
                    BuildConfiguration = new()
                    {
                        Configuration = Configuration.Development,
                        Platform = TargetPlatform.Current
                    }
                };

                s_Rules = new TargetRules(targetInfo, "Development Editor");
            }

            return s_Rules;
        }
    }
}
