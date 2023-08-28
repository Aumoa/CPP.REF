// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Data;
using System.Diagnostics;
using System.Text.Json.Nodes;

using AE.BuildSettings;
using AE.Exceptions;
using AE.Rules;
using AE.SourceTree;

namespace AE.Platform.Windows;

public class ClCompiler : Compiler
{
    private readonly string Cl;
    private readonly VisualStudioInstallation ToolChain;

    public ClCompiler(string Cl, VisualStudioInstallation Installation)
    {
        this.Cl = Cl;
        this.ToolChain = Installation;
    }

    private ProcessStartInfo MakeBaseProcessStartInfo(TargetRules Rule)
    {
        string Cl = this.Cl;
        var Config = Rule.Target.BuildConfiguration;
        foreach (var ExecutablePath in ToolChain.GetRequiredExecutablePaths(Config.Platform.Architecture))
        {
            string Cur = Path.Combine(ExecutablePath, this.Cl);
            if (File.Exists(Cur))
            {
                Cl = Cur;
                break;
            }
        }

        ProcessStartInfo PSI = new(Cl);
        string[] Paths = ToolChain.GetRequiredExecutablePaths(Config.Platform.Architecture);
        PSI.Environment["PATH"] = string.Join(';', Paths);
        string[] Includes = ToolChain.GetRequiredIncludePaths(Config.Platform.Architecture);
        PSI.Environment["INCLUDE"] = string.Join(';', Includes);
        PSI.RedirectStandardOutput = true;
        PSI.RedirectStandardError = true;

        return PSI;
    }

    private void SetBaseCompilerParameters(ProcessStartInfo PSI)
    {
        PSI.Arguments +=
            // Suppresses display of sign-on banner.
            "/nologo " +
            // Compiles without linking.
            "/c " +
            // C++20 standard ISO/IEC 14882:2020.
            "/std:c++20 " +
            // Enable C++ exception handling (no SEH exceptions) and extern "C" defaults to nothrow.
            "/EHsc " +
            // wchar_t is a native type, not a typedef.
            "/Zc:wchar_t " +
            // Checks buffer security.
            "/GS " +
            // Remove unreferenced functions or data if they're COMDAT or have internal linkage only.
            "/Zc:inline " +
            // Set output warning level to 4.
            "/W4 " +
            // Enable more security features and warnings.
            "/sdl " +
            // Disable treat warnings as errors.
            "/WX- " +
            // Set standard-conformance mode: no permissive
            "/permissive- " +
            // Displays the full path of source code files passed to cl.exe in diagnostic text.
            "/FC " +
            // Diagnostics format: prints column information.
            "/diagnostics:column " +
            // Use standard preprocessor.
            "/Zc:preprocessor "
            ;
    }

    private void SetConfigCompilerParameters(ProcessStartInfo PSI, TargetRules Rule)
    {
        switch (Rule.Target.BuildConfiguration.Configuration)
        {
            case Configuration.Debug:
            case Configuration.DebugGame:
                PSI.Arguments +=
                    // Generates intrinsic functions
                    "/Oi- " +
                    // Disable optimization.
                    "/Od " +
                    // Generates complete debugging information.
                    "/Zi " +
                    // Enable fast runtime checks.
                    "/RTC1 " +
                    // Multithreaded DLL
                    "/MDd "
                    ;
                break;
            case Configuration.Development:
            case Configuration.Shipping:
                PSI.Arguments +=
                    // Generates intrinsic functions
                    "/Oi " +
                    // Enables function-level linking.
                    "/Gy " +
                    // Generates complete debugging information.
                    "/Zi " +
                    // Creates fast code.
                    "/O2 " +
                    // Multithreaded DLL
                    "/MD "
                    ;
                break;
        }
    }

    private void SetModuleParameters(ProcessStartInfo PSI, MakefileCompile Item, TargetRules Rule)
    {
        // Appending additional arguments.
        //string GeneratedInclude = Path.Combine(Item.Intermediate, "Includes", Item.ModuleName);
        string IncludePaths = string.Join(' ', Item.ModuleInfo.IncludePaths.Select(p => $"/I\"{p}\""));

        List<string> Macros = new();
        Macros.AddRange(GenerateBuildMacros(Rule).Select(p => $"/D\"{p.Item1}\"=\"{p.Item2}\""));
        Macros.AddRange(Item.ModuleInfo.AdditionalMacros.Select(p => $"/D\"{p}\""));
        Macros.AddRange(Item.ModuleInfo.DependModules.Select(p => $"/D\"{p.ToUpper()}_API\"=\"__declspec(dllimport)\""));
        Macros.Add($"/D\"{Item.ModuleInfo.Name.ToUpper()}_API\"=\"__declspec(dllexport)\"");
        Macros.Add("/DUNICODE");
        Macros.Add("/D_UNICODE");

        PSI.Arguments += $"{IncludePaths} ";

        string AdditionalMacros = string.Join(' ', Macros);
        PSI.Arguments += $"{AdditionalMacros} ";

        string DisableWarnings = string.Join(' ', Item.ModuleInfo.DisableWarnings.Select(p => $"/wd{p}"));
        PSI.Arguments += $"{DisableWarnings} ";
    }

    private void SetSourceCodeParameters(ProcessStartInfo PSI, TargetRules Rule, MakefileCompile Item, out string OutputPath, out string DependenciesJson)
    {
        string SourceCodeName = Path.GetFileName(Item.Path);
        string Output = Path.Combine(Item.GetIntermediateOutputPath(Rule), SourceCodeName + ".obj");
        string Deps = Path.Combine(Item.GetIntermediateOutputPath(Rule), SourceCodeName + ".deps.json");
        string IntermediateOutputPath = Item.GetIntermediateOutputPath(Rule);
        string Pdb = Path.Combine(IntermediateOutputPath, SourceCodeName + ".pdb");

        PSI.Arguments += $"/Fo\"{Output}\" /Fd\"{Pdb}\" /sourceDependencies \"{Deps}\" ";

        OutputPath = Output;
        DependenciesJson = Deps;
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
        SetSourceCodeParameters(PSI, Rule, Item, out string OutputPath, out string DependenciesJson);

        if (Item.ModuleInfo.DependModules.Contains("Core"))
        {
            // Force include CoreMinimal.h
            PSI.Arguments += $"/FICoreMinimal.h ";
        }

        // Include source.
        PSI.Arguments += $"\"{Item.Path}\"";

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
                Report += Stdout;
            }
            throw new TerminateException(KnownErrorCode.CompileError, Report);
        }

        await GenerateSourceCodeCache(Item, Rule, OutputPath, DependenciesJson);
        var Elapsed = DateTime.Now - StartTime;
        return $"{Stdout.Trim()} ({Elapsed.TotalSeconds:0.00}s)";
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

    private async Task<SourceCodeCache[]> ReadDependenciesAsync(MakefileCompile Item, TargetRules Rule, string Deps)
    {
        string JsonStr = await File.ReadAllTextAsync(Deps);
        JsonNode? Node = JsonNode.Parse(JsonStr);
        JsonArray? IncludesArr = Node?["Data"]?["Includes"]?.AsArray();
        if (IncludesArr == null)
        {
            return Array.Empty<SourceCodeCache>();
        }

        string[] BuiltInPaths = ToolChain.GetRequiredIncludePaths(Rule.Target.BuildConfiguration.Platform.Architecture);
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
        return Includes.Where(p => !IsBuiltIn(p)).Select(p => SourceCodeCache.Generate(p)).ToArray();
    }
}
