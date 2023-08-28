// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Text.Json.Nodes;

using AE.BuildSettings;
using AE.Exceptions;
using AE.Rules;
using AE.SourceTree;

namespace AE.Platform.Linux;

public class GccCompiler : Compiler
{
    private readonly LinuxToolChain ToolChain;
    private readonly string CompilerPath;

    public GccCompiler(LinuxToolChain ToolChain, string CompilerPath)
    {
        this.ToolChain = ToolChain;
        this.CompilerPath = CompilerPath;
    }

    private ProcessStartInfo MakeBaseProcessStartInfo(TargetRules Rule)
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

    private void SetConfigCompilerParameters(ProcessStartInfo PSI, TargetRules Rule)
    {
        switch (Rule.Target.BuildConfiguration.Configuration)
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

    private void SetModuleParameters(ProcessStartInfo PSI, MakefileCompile Item, TargetRules Rule)
    {
        string IncludePaths = string.Join(' ', Item.ModuleInfo.IncludePaths.Select(p => $"-I\"{p}\""));

        List<string> Macros = new();
        Macros.AddRange(GenerateBuildMacros(Rule).Select(AsMacroDef));
        Macros.AddRange(Item.ModuleInfo.AdditionalMacros.Select(AsMacroDef));
        Macros.AddRange(Item.ModuleInfo.DependModules.Select(p => ($"{p.ToUpper()}_API", string.Empty)).Select(AsMacroDef));
        Macros.Add(AsMacroDef(($"{Item.ModuleInfo.Name.ToUpper()}_API", "__attribute__((visibility(\\\"default\\\")))")));
        Macros.Add("-DUNICODE");
        Macros.Add("-D_UNICODE");

        PSI.Arguments += $"{IncludePaths} ";

        string AdditionalMacros = string.Join(' ', Macros);
        PSI.Arguments += $"{AdditionalMacros} ";
    }

    private string SetSourceCodeParameters(ProcessStartInfo PSI, TargetRules Rule, MakefileCompile Item, out string OutputPath)
    {
        string SourceCodeName = Path.GetFileName(Item.Path);
        string Output = Path.Combine(Item.GetIntermediateOutputPath(Rule), SourceCodeName + ".o");

        OutputPath = Output;
        return $"-o\"{Output}\" ";
    }

    private static string AsMacroDef(string Name)
    {
        return $"-D\"{Name}\"";
    }

    private static string AsMacroDef((string, string) NameValuePair)
    {
        return $"-D\"{NameValuePair.Item1}\"=\"{NameValuePair.Item2}\"";
    }

    public override async Task<string> CompileAsync(CompileNode Node, TargetRules Rule, CancellationToken SToken = default)
    {
        ProcessStartInfo PSI = MakeBaseProcessStartInfo(Rule);
        MakefileCompile Item = Node.Compile;

        string Output = Item.GetIntermediateOutputPath(Rule);
        if (Directory.Exists(Output) == false)
        {
            Directory.CreateDirectory(Output);
        }
        SetBaseCompilerParameters(PSI);
        SetConfigCompilerParameters(PSI, Rule);
        SetModuleParameters(PSI, Item, Rule);
        string AppendStr = SetSourceCodeParameters(PSI, Rule, Item, out string OutputPath);

        if (Item.ModuleInfo.DependModules.Contains("Core"))
        {
            // Force include CoreMinimal.h
            PSI.Arguments += $"-include CoreMinimal.h ";
        }

        // Include source.
        PSI.Arguments += $"\"{Item.Path}\" ";

        string BaseArguments = PSI.Arguments;
        PSI.Arguments += AppendStr;

        DateTime StartTime = DateTime.Now;
        Process? P = Process.Start(PSI);
        if (P == null)
        {
            throw new InvalidOperationException("Internal error.");
        }

        var (Stdout, Stderr) = await GetProcessOutputsAsync(Item.Path, P, SToken);
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
        (Stdout, Stderr) = await GetProcessOutputsAsync(Item.Path, P, SToken);

        if (P.ExitCode == 0)
        {
            await GenerateSourceCodeCache(Item, Rule, OutputPath, Stdout + Stderr);
        }

        var Elapsed = DateTime.Now - StartTime;
        return $"{Path.GetFileName(Item.Path)} ({Elapsed.TotalSeconds:0.00}s)";
    }

    private async Task GenerateSourceCodeCache(MakefileCompile Item, TargetRules Rule, string ObjectOutput, string Deps)
    {
        MakefileSourceCache NewCache = new()
        {
            SourceCache = SourceCodeCache.Generate(Item.Path),
            Includes = await ReadDependenciesAsync(Item, Rule, Deps),
            ObjectOutputFile = ObjectOutput,
        };

        Item.Cache = NewCache;
    }

    private Task<SourceCodeCache[]> ReadDependenciesAsync(MakefileCompile Item, TargetRules Rule, string Deps)
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
