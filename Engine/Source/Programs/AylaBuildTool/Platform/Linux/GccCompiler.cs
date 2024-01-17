// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.BuildSettings;
using AE.CompilerServices;
using AE.CompilerServices.Makefiles;
using AE.Exceptions;

namespace AE.Platform.Linux;

public class GccCompiler(LinuxToolChain toolChain, string compilerPath) : Compiler
{
    private readonly LinuxToolChain ToolChain = toolChain;
    private readonly string CompilerPath = compilerPath;

    private ProcessStartInfo MakeBaseProcessStartInfo()
    {
        ProcessStartInfo PSI = new(CompilerPath);
        PSI.RedirectStandardOutput = true;
        PSI.RedirectStandardError = true;
        return PSI;
    }

    private void SetBaseCompilerParameters(ProcessStartInfo PSI)
    {
        PSI.Arguments +=
            // Compiles without linking.
            "-c " +
            // C++20 standard ISO/IEC 14882:2020.
            "-std=c++20 " +
            // Supports concepts
            "-fconcepts " +
            // Supports coroutines
            "-fcoroutines " +
            // Supports shared library.
            "-fPIC "
            ;
    }

    private void SetConfigCompilerParameters(ProcessStartInfo PSI)
    {
        switch (Target.Configuration)
        {
            case Configuration.Debug:
            case Configuration.DebugGame:
                PSI.Arguments +=
                    // Generates debugging information.
                    "-g "
                    ;
                break;
            case Configuration.Development:
            case Configuration.Shipping:
                break;
        }
    }

    private static string AsCompilerInclude(string InPath)
    {
        return $"-I\"{InPath}\"";
    }

    private static string AsCompilerMacro((string, string?) InPair)
    {
        if (InPair.Item2 == null)
        {
            return $"-D\"{InPair.Item1}\"";
        }
        else
        {
            return $"-D\"{InPair.Item1}\"=\"{InPair.Item2}\"";
        }
    }

    private void SetModuleParameters(ProcessStartInfo PSI, MakefileCompile Item)
    {
        var Includes = Item.CollectCompilerIncludePaths().Select(AsCompilerInclude);
        var Macros = Item.CollectCompilerMacros().Select(AsCompilerMacro);

        string IncludeStr = string.Join(' ', Includes);
        string MacroStr = string.Join(' ', Macros);

        PSI.Arguments += $"{IncludeStr} {MacroStr} ";
    }

    private string SetSourceCodeParameters(ProcessStartInfo PSI, MakefileCompile Item, out string OutputPath)
    {
        string SourceCodeName = Path.GetFileName(Item.SourceFile);
        string Output = Path.Combine(Item.BuildsOut(), SourceCodeName + ".o");

        OutputPath = Output;
        return $"-o\"{Output}\" ";
    }

    public override async Task<string> CompileAsync(MakefileCompile Node, CancellationToken SToken = default)
    {
        ProcessStartInfo PSI = MakeBaseProcessStartInfo();

        string Output = Node.BuildsOut();
        if (Directory.Exists(Output) == false)
        {
            Directory.CreateDirectory(Output);
        }
        SetBaseCompilerParameters(PSI);
        SetConfigCompilerParameters(PSI);
        SetModuleParameters(PSI, Node);
        string AppendStr = SetSourceCodeParameters(PSI, Node, out string OutputPath);

        if (Node.ModuleInfo.DependModules.Contains("Core"))
        {
            // Force include CoreMinimal.h
            PSI.Arguments += $"-include CoreMinimal.h ";
        }

        // Include source.
        PSI.Arguments += $"\"{Node.SourceFile}\" ";

        string BaseArguments = PSI.Arguments;
        PSI.Arguments += AppendStr;

        DateTime StartTime = DateTime.Now;
        Process? P = Process.Start(PSI);
        if (P == null)
        {
            throw new InvalidOperationException("Internal error.");
        }

        var (Stdout, Stderr) = await GetProcessOutputsAsync(Node.SourceFile, P, SToken);
        if (P.ExitCode != 0)
        {
            string Report = string.Empty;
            if (P.ExitCode != 0)
            {
                Report += Stderr;
            }
            Console.Error.WriteLine(PSI.Arguments);
            throw new TerminateException(KnownErrorCode.CompileError, Report);
        }

        PSI.Arguments = BaseArguments + " -M";
        P = Process.Start(PSI);
        if (P == null)
        {
            throw new InvalidOperationException("Internal error.");
        }
        (Stdout, Stderr) = await GetProcessOutputsAsync(Node.SourceFile, P, SToken);

        if (P.ExitCode == 0)
        {
            await GenerateSourceCodeCache(Node, OutputPath, Stdout + Stderr);
        }

        var Elapsed = DateTime.Now - StartTime;
        return $"{Path.GetFileName(Node.SourceFile)} ({Elapsed.TotalSeconds:0.00}s)";
    }

    private async Task GenerateSourceCodeCache(MakefileCompile Item, string ObjectOutput, string Deps)
    {
        MakefileSourceCache NewCache = new()
        {
            SourceCache = SourceCodeCache.Generate(Item.SourceFile),
            Dependencies = await ReadDependenciesAsync(Item, Deps),
            ObjectOutputFile = ObjectOutput,
        };

        Item.Cache = NewCache;
    }

    private Task<SourceCodeCache[]> ReadDependenciesAsync(MakefileCompile Item, string Deps)
    {
        string[] IncludesArr = Deps.Split('\\', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);

        // exclude *.o(out) and *.cpp(self)
        if (IncludesArr.Length < 2)
        {
            return Task.FromResult(Array.Empty<SourceCodeCache>());
        }

        IncludesArr = IncludesArr[2..];

        string[] BuiltInPaths = new[] { "/usr/lib", "/usr/include" };
        bool IsBuiltIn(string InPath)
        {
            foreach (var BuiltInPath in BuiltInPaths)
            {
                if (InPath.StartsWith(BuiltInPath, StringComparison.OrdinalIgnoreCase))
                {
                    return true;
                }
            }

            return false;
        }

        string[] Includes = IncludesArr.Select(p => p!.ToString()).ToArray();
        return Task.FromResult(Includes.Where(p => !IsBuiltIn(p)).Select(p => SourceCodeCache.Generate(p)).ToArray());
    }

    private static async Task<(string, string)> GetProcessOutputsAsync(string DebugString, Process P, CancellationToken SToken = default)
    {
        _ = DebugString;

        async Task<string> AsyncReader(StreamReader Reader, CancellationToken SToken)
        {
            List<string> Lines = new();

            while (SToken.IsCancellationRequested == false)
            {
                // Do NOT pass CancellationToken to ReadLineAsync function.
                string? Line = await Reader.ReadLineAsync();
                if (Line != null)
                {
                    Lines.Add(Line);
                }
            }

            string? Last = await Reader.ReadToEndAsync();
            if (Last != null)
            {
                Lines.Add(Last);
            }
            return string.Join('\n', Lines);
        }

        var CTS = new CancellationTokenSource();
        Task<string> StdoutReader = AsyncReader(P.StandardOutput, CTS.Token);
        Task<string> StderrReader = AsyncReader(P.StandardError, CTS.Token);

        await P.WaitForExitAsync(SToken);
        CTS.Cancel();
        await Task.WhenAll(StdoutReader, StderrReader);
        return (StdoutReader.Result, StderrReader.Result);
    }
}
