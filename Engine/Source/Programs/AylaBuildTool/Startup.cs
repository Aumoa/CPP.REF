// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Reflection;
using System.Text;

using AE.BuildSettings;
using AE.CLI;
using AE.Diagnostics;
using AE.Exceptions;
using AE.Executors;
using AE.Misc;
using AE.Platform;
using AE.Projects;
using AE.Rules;
using AE.Source;

if (args.IsValidIndex(0) == false)
{
    Console.Error.WriteLine("Argument error. First argument must be specify executor.");
    return -1;
}

string ExecutorName = args[0];
Type? ExecutorType = typeof(IExecutor).Assembly.GetType("AE.Executors." + ExecutorName + "Executor");
if (ExecutorType == null)
{
    Console.Error.WriteLine("Executor {0} is not valid. Cannot find class.", ExecutorName);
    return -1;
}

if (ExecutorType.IsAssignableTo(typeof(IExecutor)) == false)
{
    Console.Error.WriteLine("Executor {0} is not valid. Is not implements IExecutor interface.", ExecutorName);
    return -1;
}

ConstructorInfo? Constructor = ExecutorType.GetConstructor(new[] { typeof(CommandLineParser) });
if (Constructor == null)
{
    Console.Error.WriteLine("Executor {0} is not valid. Cannot find default constructor.", ExecutorName);
    return -1;
}

CancellationTokenSource CTS = new();
Console.CancelKeyPress += (Sender, EventArgs) =>
{
    CTS.Cancel();
};

try
{
    Console.WriteLine(string.Join(' ', args));
    var Executor = (IExecutor)Constructor.Invoke(new object[] { CommandLineParser.Parse(args[1..]) });
    return await Executor.RunAsync(CTS.Token);
}
catch (TerminateException TE)
{
    Console.Error.WriteLine(TE.Message);
    return (int)TE.ErrorCode;
}
catch (AggregateException AE)
{
    if (AE.InnerExceptions.Any())
    {
        List<string> CompilerMessages = new();
        List<Exception> NewExceptionList = new();
        foreach (var Exception in AE.InnerExceptions)
        {
            if (Exception is TerminateException TE)
            {
                if (TE.ErrorCode == KnownErrorCode.CompileError)
                {
                    CompilerMessages.Add(Exception.Message);
                }
                else
                {
                    NewExceptionList.Add(Exception);
                }
            }
            else
            {
                NewExceptionList.Add(Exception);
            }
        }

        if (CompilerMessages.Any())
        {
            Console.Error.Write(string.Join('\n', CompilerMessages));
        }
        if (NewExceptionList.Any())
        {
            throw;
        }
        else
        {
            return (int)KnownErrorCode.CompileError;
        }
    }
    throw;
}