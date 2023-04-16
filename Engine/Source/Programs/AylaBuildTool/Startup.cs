// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Platform;
using AE.Projects;
using AE.Rules;
using AE.Source;

internal static class Startup
{
    public static async Task<int> Main(string[] Args)
    {
        CancellationTokenSource Source = new();
        Console.CancelKeyPress += (Sender, Args) =>
        {
            Source.Cancel();
        };

        var EngineWorkspace = new ProjectDirectory { Root = "F:\\CPP.REF\\Engine" };
        var GameWorkspace = new ProjectDirectory { Root = "F:\\SampleGame" };

        EngineWorkspace.GenerateDirectoriesRecursive();
        GameWorkspace.GenerateDirectoriesRecursive();

        TargetInfo Target = new()
        {
            BuildConfiguration = new()
            {
                Configuration = AE.BuildSettings.Configuration.Shipping,
                Platform = TargetPlatform.Win64
            },
            bEditor = true
        };

        var Resolver = new ModuleDependenciesResolver(EngineWorkspace, GameWorkspace);
        var Rule = await Resolver.ResolveAsync("SampleGame", Target, Source.Token);

        var Compiler = new ModuleCompiler();
        var SourceTree = new CompileTree(EngineWorkspace);

        Console.WriteLine("Scan dependencies...");

        List<Task> Tasks = new();
        foreach (var (_, Module) in Resolver.RequiredModules)
        {
            Tasks.Add(Module.ScanDependenciesAsync(Rule, Compiler, SourceTree, Resolver, Source.Token));
        }
        await Task.WhenAll(Tasks);

        await SourceTree.CompileAsync(Compiler, Source.Token);
        return 0;
    }

    //public static async Task<int> Main(string[] args)
    //{
    //    if (args.IsValidIndex(0) == false)
    //    {
    //        Log.Error("Argument error. First argument must be specify executor.");
    //        return -1;
    //    }

    //    string ExecutorName = args[0];
    //    Type? ExecutorType = typeof(Startup).Assembly.GetType("AE.Executors." + ExecutorName + "Executor");
    //    if (ExecutorType == null)
    //    {
    //        Log.Error("Executor {0} is not valid. Cannot find class.", ExecutorName);
    //        return -1;
    //    }

    //    if (ExecutorType.IsAssignableTo(typeof(IExecutor)) == false)
    //    {
    //        Log.Error("Executor {0} is not valid. Is not implements IExecutor interface.", ExecutorName);
    //        return -1;
    //    }

    //    ConstructorInfo? Constructor = ExecutorType.GetConstructor(new[] { typeof(CommandLineParser) });
    //    if (Constructor == null)
    //    {
    //        Log.Error("Executor {0} is not valid. Cannot find default constructor.", ExecutorName);
    //        return -1;
    //    }

    //    CancellationTokenSource CTS = new();
    //    Console.CancelKeyPress += (Sender, EventArgs) =>
    //    {
    //        CTS.Cancel();
    //    };

    //    var Executor = (IExecutor)Constructor.Invoke(new object[] { CommandLineParser.Parse(args[1..]) });
    //    return await Executor.RunAsync(CTS.Token);
    //}
}
