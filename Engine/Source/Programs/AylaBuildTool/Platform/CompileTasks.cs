// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Rules;
using AE.SourceTree;
using AE.System;

namespace AE.Platform;

public class CompileTasks : Compiler
{
    private readonly ToolChainInstallation ToolChain;
    private readonly SemaphoreSlim Access;

    public CompileTasks(ToolChainInstallation ToolChain, int MaxParallel = 0)
    {
        this.ToolChain = ToolChain;
        if (MaxParallel == 0)
        {
            MaxParallel = Environment.ProcessorCount;
        }
        this.MaxParallel = MaxParallel;
        Access = new SemaphoreSlim(MaxParallel);
    }

    public int MaxParallel { get; private init; }

    public override async Task<string> CompileAsync(MakefileCompile Item, TargetRules Rule, CancellationToken SToken = default)
    {
        await Access.WaitAsync();
        try
        {
            var Compiler = ToolChain.SpawnCompiler();
            return await Compiler.CompileAsync(Item, Rule, SToken);
        }
        finally
        {
            Access.Release();
        }
    }
}
