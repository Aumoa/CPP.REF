﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CompilerServices;
using AE.CompilerServices.Makefiles;

namespace AE.Platform;

public class CompileTasks : Compiler
{
    private readonly SemaphoreSlim Access;

    public CompileTasks(int MaxParallel = 0)
    {
        if (MaxParallel == 0)
        {
            MaxParallel = Environment.ProcessorCount;
        }
        this.MaxParallel = MaxParallel;
        Access = new SemaphoreSlim(MaxParallel);
    }

    public int MaxParallel { get; private init; }

    public override async Task<string> CompileAsync(MakefileCompile Node, CancellationToken SToken = default)
    {
        await Access.WaitAsync();
        try
        {
            var Compiler = ToolChain.SpawnCompiler();
            return await Compiler.CompileAsync(Node, SToken);
        }
        finally
        {
            Access.Release();
        }
    }
}
