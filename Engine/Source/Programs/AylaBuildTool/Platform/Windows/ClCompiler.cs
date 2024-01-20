// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Data;
using System.Diagnostics;
using System.Text.Json.Nodes;

using AE.AppHelper;
using AE.BuildSettings;
using AE.CompilerServices;
using AE.CompilerServices.Makefiles;
using AE.Exceptions;
using AE.IO;

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

    private ProcessStartInfo MakeBaseProcessStartInfo()
    {
        string Cl = this.Cl;
        var Config = Target.Config;
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

    private void SetConfigCompilerParameters(ProcessStartInfo PSI)
    {
        switch (Target.Configuration)
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

    private static string AsCompilerInclude(string InPath)
    {
        return $"/I\"{InPath}\"";
    }

    private static string AsCompilerMacro((string, string?) InPair)
    {
        if (InPair.Item2 == null)
        {
            return $"/D\"{InPair.Item1}\"";
        }
        else
        {
            return $"/D\"{InPair.Item1}\"=\"{InPair.Item2.Replace("\"", "\\\"")}\"";
        }
    }

    private void SetModuleParameters(ProcessStartInfo PSI, MakefileCompile Item)
    {
        var Includes = Item.CollectCompilerIncludePaths().Select(AsCompilerInclude);
        var Macros = Item.CollectCompilerMacros().Select(AsCompilerMacro);

        string IncludeStr = string.Join(' ', Includes);
        string MacroStr = string.Join(' ', Macros);

        PSI.Arguments += $"{IncludeStr} {MacroStr} ";

        string DisableWarnings = string.Join(' ', Item.ModuleInfo.PrivateDisableWarnings.Select(p => $"/wd{p}"));
        PSI.Arguments += $"{DisableWarnings} ";
    }

    private void SetSourceCodeParameters(ProcessStartInfo PSI, MakefileCompile Item, out string OutputPath, out string DependenciesJson)
    {
        string SourceCodeName = Path.GetFileName(Item.SourceFile);
        string Output = Path.Combine(Item.BuildsOut(), SourceCodeName + ".obj");
        string Deps = Path.Combine(Item.BuildsOut(), SourceCodeName + ".deps.json");
        string IntermediateOutputPath = Item.BuildsOut();
        string Pdb = Path.Combine(IntermediateOutputPath, SourceCodeName + ".pdb");

        PSI.Arguments += $"/Fo\"{Output}\" /Fd\"{Pdb}\" /sourceDependencies \"{Deps}\" ";

        OutputPath = Output;
        DependenciesJson = Deps;
    }

    public override async Task<string> CompileAsync(MakefileCompile node, CancellationToken cancellationToken = default)
    {
        ProcessStartInfo psi = MakeBaseProcessStartInfo();

        DirectoryReference output = node.BuildsOut();
        if (output.IsExists == false)
        {
            output.Create();
        }
        SetBaseCompilerParameters(psi);
        SetConfigCompilerParameters(psi);
        SetModuleParameters(psi, node);
        SetSourceCodeParameters(psi, node, out string outputPath, out string dependenciesJson);

        if (node.ModuleInfo.DependModules.Contains("Core"))
        {
            // Force include CoreMinimal.h
            psi.Arguments += $"/FICoreMinimal.h ";
        }

        // Include source.
        psi.Arguments += $"\"{node.SourceFile}\"";

        DateTime startTime = DateTime.Now;
        var app = await App.Run(psi, cancellationToken: cancellationToken);
        if (app.ExitCode != 0)
        {
            string report = string.Empty;
            if (app.ExitCode != 0)
            {
                report += app.Stdout;
            }
            throw new TerminateException(KnownErrorCode.CompileError, report);
        }

        await GenerateSourceCodeCache(node, outputPath, dependenciesJson);
        var elapsed = DateTime.Now - startTime;
        return $"{app.Stdout.Trim()} ({elapsed.TotalSeconds:0.00}s)";
    }

    private async Task GenerateSourceCodeCache(MakefileCompile node, string objectOutput, string deps)
    {
        MakefileSourceCache NewCache = new()
        {
            SourceCache = SourceCodeCache.Generate(node.SourceFile),
            Dependencies = await ReadDependenciesAsync(node, deps),
            ObjectOutputFile = objectOutput,
        };

        node.Cache = NewCache;
    }

    private async Task<SourceCodeCache[]> ReadDependenciesAsync(MakefileCompile Item, string Deps)
    {
        string JsonStr = await File.ReadAllTextAsync(Deps);
        JsonNode? Node = JsonNode.Parse(JsonStr);
        JsonArray? IncludesArr = Node?["Data"]?["Includes"]?.AsArray();
        if (IncludesArr == null)
        {
            return Array.Empty<SourceCodeCache>();
        }

        string[] BuiltInPaths = ToolChain.GetRequiredIncludePaths(Target.Architecture);
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
