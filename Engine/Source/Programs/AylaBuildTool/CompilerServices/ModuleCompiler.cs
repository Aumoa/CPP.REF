// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public static class ModuleCompiler
{
    public static async Task<bool> CompileAsync(string targetName, CancellationToken cancellationToken = default)
    {
        if (Workspace.TryFindModule(targetName, out var assembly) == false)
        {
            throw new TerminateException(KnownErrorCode.TargetNotFound, Locale.Errors.TargetNotFoundException(targetName));
        }

        MakefileGenerator.Clear();
        ToolChain.Init(ToolChainInstallation.Spawn(Target.Info));

        Console.Write("Generate makefiles for {0}...", targetName);
        ScopedTimer.Start();
        await MakefileGenerator.GenerateAsync(cancellationToken);
        Console.WriteLine(" done. ({0:F4} seconds elapsed)", ScopedTimer.Stop().TotalSeconds);

        await using var savingScope = new MakefileGenerator.SavingScope();
        var context = MakefileContext.Create(MakefileGenerator.Makefiles);

        return await DispatchCompileWorkers(context, cancellationToken);
    }

    private static async Task<bool> DispatchCompileWorker(MakefileContext context, MakefileCompile makefileNode, CancellationToken cancellationToken)
    {
        try
        {
            await makefileNode.DependsOn.WaitAsync(cancellationToken);

            string stdout = await CompileTasks.CompileAsync(makefileNode, cancellationToken);
            makefileNode.MarkCompleted();
            string count = Interlocked.Increment(ref context.CompiledCount).ToString().PadLeft(context.TotalCountStr.Length);
            Console.WriteLine("[{0}/{1}] {2}", count, context.TotalCountStr, stdout);
            return true;
        }
        catch (TerminateException terminate)
        {
            makefileNode.MarkCanceled();
            Console.Error.WriteLine(terminate.Message);
            return false;
        }
        catch (Exception e)
        {
            if (e is OperationCanceledException or TaskCanceledException)
            {
                makefileNode.MarkCanceled();
            }
            else
            {
                makefileNode.MarkError(e);
            }

            throw;
        }
    }

    private static async Task<bool> DispatchLinkWorker(MakefileContext context, Linker linker, MakefileLink makefileNode, CancellationToken cancellationToken)
    {
        string count;

        try
        {
            await makefileNode.DependsOn.WaitAsync(cancellationToken);

            bool cached = await linker.TryCheckOutputsAsync(makefileNode.ModuleInfo, cancellationToken);
            if (cached && makefileNode.IsRequired == false)
            {
                makefileNode.MarkCompleted();
                count = Interlocked.Increment(ref context.CompiledCount).ToString().PadLeft(context.TotalCountStr.Length);
                Console.WriteLine("[{0}/{1}] {2}", count, context.TotalCountStr, makefileNode.ModuleInfo.Name);
                return true;
            }

            string stdout = await linker.LinkAsync(makefileNode.ModuleInfo, cancellationToken);
            count = Interlocked.Increment(ref context.CompiledCount).ToString().PadLeft(context.TotalCountStr.Length);
            Console.WriteLine("[{0}/{1}] {2}", count, context.TotalCountStr, stdout);
            makefileNode.MarkCompleted();
            return true;
        }
        catch (TerminateException terminate)
        {
            makefileNode.MarkCanceled();
            Console.Error.WriteLine(terminate.Message);
            return false;
        }
        catch (Exception e)
        {
            if (e is OperationCanceledException or TaskCanceledException)
            {
                makefileNode.MarkCanceled();
            }
            else
            {
                makefileNode.MarkError(e);
            }

            throw;
        }
    }

    private static async Task<bool> DispatchShaderCompileWorker(MakefileContext context, MakefileCompile node, CancellationToken cancellationToken)
    {
        try
        {
            await node.DependsOn.WaitAsync(cancellationToken);

            string stdout = await ShaderCompileTasks.CompileAsync(node, cancellationToken);
            node.MarkCompleted();
            string count = Interlocked.Increment(ref context.CompiledCount).ToString().PadLeft(context.TotalCountStr.Length);
            Console.WriteLine("[{0}/{1}] {2}", count, context.TotalCountStr, stdout);
            return true;
        }
        catch (TerminateException terminate)
        {
            node.MarkCanceled();
            Console.Error.WriteLine(terminate.Message);
            return false;
        }
        catch (Exception e)
        {
            if (e is OperationCanceledException or TaskCanceledException)
            {
                node.MarkCanceled();
            }
            else
            {
                node.MarkError(e);
            }

            throw;
        }
    }

    private static async Task<bool> DispatchCompileWorkers(MakefileContext context, CancellationToken cancellationToken)
    {
        var tasks = new List<Task<bool>>();

        foreach (var node in context.CompileNodes)
        {
            if (node.SourceFile.IsSourceCode())
            {
                tasks.Add(DispatchCompileWorker(context, node, cancellationToken));
            }
            else if (node.SourceFile.IsShaderCode())
            {
                tasks.Add(DispatchShaderCompileWorker(context, node, cancellationToken));
            }
            else
            {
                throw new InvalidOperationException();
            }
        }

        foreach (var node in context.LinkNodes)
        {
            tasks.Add(DispatchLinkWorker(context, ToolChain.SpawnLinker(), node, cancellationToken));
        }

        var results = await Task.WhenAll(tasks);
        return results.Any(p => !p) == false;
    }
}
