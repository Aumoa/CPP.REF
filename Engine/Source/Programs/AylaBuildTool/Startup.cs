// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.CLI;
using AE.Diagnostics;
using AE.Executors;
using AE.Misc;

using System.Reflection;

internal static class Startup
{
    public static async Task<int> Main(string[] args)
    {
        if (args.IsValidIndex(0) == false)
        {
            Log.Error("Argument error. First argument must be specify executor.");
            return -1;
        }

        string ExecutorName = args[0];
        Type? ExecutorType = typeof(Startup).Assembly.GetType("AE.Executors." + ExecutorName + "Executor");
        if (ExecutorType == null)
        {
            Log.Error("Executor {0} is not valid. Cannot find class.", ExecutorName);
            return -1;
        }

        if (ExecutorType.IsAssignableTo(typeof(IExecutor)) == false)
        {
            Log.Error("Executor {0} is not valid. Is not implements IExecutor interface.", ExecutorName);
            return -1;
        }

        ConstructorInfo? Constructor = ExecutorType.GetConstructor(new[] { typeof(CommandLineParser) });
        if (Constructor == null)
        {
            Log.Error("Executor {0} is not valid. Cannot find default constructor.", ExecutorName);
            return -1;
        }

        CancellationTokenSource CTS = new();
        Console.CancelKeyPress += (Sender, EventArgs) =>
        {
            CTS.Cancel();
        };

        var Executor = (IExecutor)Constructor.Invoke(new object[] { CommandLineParser.Parse(args[1..]) });
        return await Executor.RunAsync(CTS.Token);
    }
}
