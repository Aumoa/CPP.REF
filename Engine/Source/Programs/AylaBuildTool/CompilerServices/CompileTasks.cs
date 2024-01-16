// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CompilerServices.Makefiles;
using AE.Platform;

namespace AE.CompilerServices;

public static class CompileTasks
{
    private static TaskPipeline<string, MakefileCompile> s_Pipeline;

    static CompileTasks()
    {
        s_Pipeline = new TaskPipeline<string, MakefileCompile>(DoCompileAsync);
    }

    public static Task<string> CompileAsync(MakefileCompile node, CancellationToken cancellationToken = default)
    {
        return s_Pipeline.PublishAsync(node, cancellationToken);
    }

    private static Task<string> DoCompileAsync(MakefileCompile node, CancellationToken cancellationToken)
    {
        var compiler = ToolChain.SpawnCompiler();
        return compiler.CompileAsync(node, cancellationToken);
    }
}
