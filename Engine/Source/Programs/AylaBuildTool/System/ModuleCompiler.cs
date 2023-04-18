// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Exceptions;
using AE.Platform;
using AE.Platform.Windows;

using System.Diagnostics;

namespace AE.System;

public class ModuleCompiler
{
    public readonly ToolChainInstallation ToolChain;
    public readonly string Executable;
    public readonly int MaxParallelCompilers;

    public ModuleCompiler(ToolChainInstallation ToolChain, string Executable, int MaxParallelCompilers = 0)
    {
        Debug.Assert(MaxParallelCompilers >= 0);
        if (MaxParallelCompilers == 0)
        {
            MaxParallelCompilers = Process.GetCurrentProcess().Threads.Count;
        }

        this.ToolChain = ToolChain;
        this.Executable = SearchExecutable(Executable);
        this.MaxParallelCompilers = MaxParallelCompilers;
    }

    private readonly Dictionary<string, SemaphoreSlim> ProcessorLimits = new();
    private readonly Dictionary<string, Task<string>> SourceCodeCompiles = new();

    private string SearchExecutable(string Executable)
    {
        foreach (var ExecutablePath in ToolChain.GetRequiredExecutablePaths(Architecture.x64))
        {
            string Cur = Path.Combine(ExecutablePath, Executable);
            if (File.Exists(Cur))
            {
                Executable = Cur;
                break;
            }
        }

        return Executable;
    }

    public async Task<string> RunCompilerAsync(string SourceCode, string Commands, CancellationToken SToken = default)
    {
        string Stdout = string.Empty;

        TaskCompletionSource<string>? SourceCodeCompletion = null;
        Task<string>? SourceCodeTask = null!;
        lock (SourceCodeCompiles)
        {
            if (SourceCodeCompiles.TryGetValue(SourceCode, out SourceCodeTask))
            {
                SourceCodeCompletion = new();
                SourceCodeCompiles.Add(SourceCode, SourceCodeTask);
            }
        }

        if (SourceCodeTask == null)
        {
            SemaphoreSlim? Semaphore;
            lock (ProcessorLimits)
            {
                if (ProcessorLimits.TryGetValue(Executable, out Semaphore) == false)
                {
                    Semaphore = new SemaphoreSlim(MaxParallelCompilers, MaxParallelCompilers);
                    ProcessorLimits.Add(Executable, Semaphore);
                }
            }

            await Semaphore.WaitAsync(SToken);

            try
            {
                string Paths = string.Join(';', ToolChain.GetRequiredExecutablePaths(Architecture.x64));
                string Includes = string.Join(';', ToolChain.GetRequiredIncludePaths(Architecture.x64));

                ProcessStartInfo PSI = new(Executable);
                PSI.Environment["PATH"] = Paths;
                PSI.Environment["INCLUDE"] = Includes;
                PSI.Arguments = Commands;
                PSI.RedirectStandardOutput = true;
                PSI.RedirectStandardError = true;
                Process? P = Process.Start(PSI);
                if (P == null)
                {
                    throw new InvalidOperationException();
                }

                Stdout = await P.StandardOutput.ReadToEndAsync();
                string Stderr = await P.StandardError.ReadToEndAsync();
                await P.WaitForExitAsync(SToken);
                Stdout += await P.StandardOutput.ReadToEndAsync();
                Stderr += await P.StandardError.ReadToEndAsync();

                if (P.ExitCode != 0)
                {
                    throw new TerminateException(9, Stdout + Stderr);
                }

                return Stdout;
            }
            finally
            {
                Semaphore.Release();
                SourceCodeCompletion?.SetResult(Stdout);
            }
        }
        else
        {
            return await SourceCodeTask;
        }
    }
}
