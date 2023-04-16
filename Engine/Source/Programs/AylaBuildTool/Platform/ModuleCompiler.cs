// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Platform.Windows;

using System.Diagnostics;

namespace AE.Platform;

public class ModuleCompiler
{
    public readonly int MaxParallelCompilers;

    public ModuleCompiler(int MaxParallelCompilers = 0)
    {
        Debug.Assert(MaxParallelCompilers >= 0);
        if (MaxParallelCompilers == 0)
        {
            MaxParallelCompilers = Process.GetCurrentProcess().Threads.Count;
        }

        this.MaxParallelCompilers = MaxParallelCompilers;
    }

    private readonly Dictionary<string, SemaphoreSlim> ProcessorLimits = new();
    private readonly Dictionary<string, Task<string>> SourceCodeCompiles = new();

    private string SearchExecutable(string Executable)
    {
        VisualStudioInstallation[] Installations = VisualStudioInstallation.FindVisualStudioInstallations(Compiler.VisualStudio2022);
        VisualStudioInstallation Installation = Installations[0];

        foreach (var ExecutablePath in Installation.GetRequiredExecutablePaths(Architecture.x64))
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
        string Executable = "cl.exe";
        string Stdout = string.Empty;

        VisualStudioInstallation[] Installations = VisualStudioInstallation.FindVisualStudioInstallations(Compiler.VisualStudio2022);
        VisualStudioInstallation Installation = Installations[0];

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
                Executable = SearchExecutable(Executable);

                string Paths = string.Join(';', Installation.GetRequiredExecutablePaths(Architecture.x64));
                string Includes = string.Join(';', Installation.GetRequiredIncludePaths(Architecture.x64));

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
                _ = await P.StandardError.ReadToEndAsync();
                await P.WaitForExitAsync(SToken);
                Stdout += await P.StandardOutput.ReadToEndAsync();
                _ = await P.StandardError.ReadToEndAsync();

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
