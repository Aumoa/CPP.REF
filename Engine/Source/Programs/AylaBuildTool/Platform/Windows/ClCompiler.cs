// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Data;
using System.Diagnostics;
using System.Text.Json.Nodes;

using AE.BuildSettings;
using AE.Compile;
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
            // Force include CoreMinimal.h
            $"/FICoreMinimal.h "
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
        string IncludePaths = string.Join(' ', Item.IncludePaths.Select(p => $"/I\"{p}\""));

        List<string> Macros = new();
        Macros.AddRange(GenerateBuildMacros(Rule).Select(p => $"/D\"{p.Item1}\"=\"{p.Item2}\""));
        Macros.AddRange(Item.AdditionalMacros.Select(p => $"/D\"{p}\""));
        Macros.AddRange(Item.DependModules.Select(p => $"/D\"{p.ToUpper()}_API\"=\"__declspec(dllimport)\""));
        Macros.Add($"/D\"{Item.ModuleName.ToUpper()}_API\"=\"__declspec(dllexport)\"");
        Macros.Add("/DUNICODE");
        Macros.Add("/D_UNICODE");

        PSI.Arguments += $"{IncludePaths} ";

        string AdditionalMacros = string.Join(' ', Macros);
        PSI.Arguments += $"{AdditionalMacros} ";

        string DisableWarnings = string.Join(' ', Item.DisableWarnings.Select(p => $"/wd{p}"));
        PSI.Arguments += $"{DisableWarnings} ";
    }

    private void SetSourceCodeParameters(ProcessStartInfo PSI, MakefileCompile Item)
    {
        string SourceCodeName = Path.GetFileName(Item.SourceCode);
        string Output = Path.Combine(Item.Cache.IntermediateOutput, SourceCodeName + ".obj");
        if (Path.HasExtension(SourceCodeName) == false)
        {
            Output = Path.ChangeExtension(Output, ".std.obj");
        }
        string Pdb = Path.Combine(Item.Cache.IntermediateOutput, SourceCodeName + ".pdb");
        string IfcOutput = Item.Cache.IntermediateOutput;

        Item.Cache.ObjectOutput = Output;
        PSI.Arguments += $"/Fo\"{Output}\" /Fd\"{Pdb}\" /ifcOutput\"{IfcOutput}\" ";
    }

    public override async Task<string> CompileAsync(CompileNode Node, TargetRules Rule, CancellationToken SToken = default)
    {
        ProcessStartInfo PSI = MakeBaseProcessStartInfo(Rule);
        MakefileCompile Item = Node.Compile;

        string Output = Item.Cache.IntermediateOutput;
        if (Directory.Exists(Output) == false)
        {
            Directory.CreateDirectory(Output);
        }
        SetBaseCompilerParameters(PSI);
        SetConfigCompilerParameters(PSI, Rule);
        SetModuleParameters(PSI, Item, Rule);
        SetSourceCodeParameters(PSI, Item);

        if (Item.Cache.Provide != null)
        {
            if (Item.Cache.Provide.IsInterface == false)
            {
                PSI.Arguments += $"/exportHeader /headerName:{Item.Cache.Provide.LookupMethod!.Value.ToString().ToLower()} ";
            }
        }

        // ifc references.
        foreach (var IfcSearchDir in Node.IfcSearchDirs)
        {
            PSI.Arguments += $"/ifcSearchDir \"{IfcSearchDir}\" ";
        }
        foreach (var HeaderImport in Node.HeaderImports)
        {
            PSI.Arguments += $"/headerUnit:angle \"{HeaderImport}\"=\"{Path.ChangeExtension(HeaderImport, ".ifc")}\" ";
        }

        // Include source.
        PSI.Arguments += $"\"{Item.SourceCode}\"";

        Process? P = Process.Start(PSI);
        if (P == null)
        {
            throw new InvalidOperationException("Internal error.");
        }

        var (Stdout, Stderr) = await GetProcessOutputsAsync(Item.SourceCode, P, SToken);
        if (P.ExitCode != 0)
        {
            string Report = string.Empty;
            if (P.ExitCode == 1 || P.ExitCode == 2)
            {
                Report += Stdout;
            }
            throw new TerminateException(KnownErrorCode.CompileError, Report);
        }

        return Stdout.Trim();
    }

    public override async Task<MakefileCompile> ScanDependenciesAsync(MakefileCompile Item, TargetRules Rule, CancellationToken SToken = default)
    {
        ProcessStartInfo PSI = MakeBaseProcessStartInfo(Rule);

        string Output = Item.Cache.IntermediateOutput;
        string Pdb = Path.Combine(Output, Path.GetFileName(Item.SourceCode) + ".pdb");
        if (Directory.Exists(Output) == false)
        {
            Directory.CreateDirectory(Output);
        }

        SetBaseCompilerParameters(PSI);
        SetConfigCompilerParameters(PSI, Rule);
        SetModuleParameters(PSI, Item, Rule);
        SetSourceCodeParameters(PSI, Item);

        // Include source.
        PSI.Arguments += $"/scanDependencies- /showIncludes /interface /TP {Item.SourceCode}";

        Process? P = Process.Start(PSI);
        if (P == null)
        {
            throw new InvalidOperationException("Internal error.");
        }

        var (Stdout, Stderr) = await GetProcessOutputsAsync(Item.SourceCode, P, SToken);
        if (P.ExitCode != 0)
        {
            Console.Error.WriteLine(Stdout + Stderr);
            return Item;
        }

        JsonNode? Root = JsonNode.Parse(Stdout);
        if (Root == null)
        {
            throw new TerminateException(KnownErrorCode.NotSupportedCompiler, CoreStrings.Errors.CompilerNotSupported);
        }

        int? Version = (int?)Root["version"];
        if (Version == null || Version.Value != 1)
        {
            throw new TerminateException(KnownErrorCode.NotSupportedCompiler, CoreStrings.Errors.CompilerNotSupported);
        }

        int? Revision = (int?)Root["revision"];
        if (Revision == null || Revision.Value != 0)
        {
            throw new TerminateException(KnownErrorCode.NotSupportedCompiler, CoreStrings.Errors.CompilerNotSupported);
        }

        JsonArray? Rules = Root["rules"] as JsonArray;
        if (Rules == null)
        {
            throw new TerminateException(KnownErrorCode.NotSupportedCompiler, CoreStrings.Errors.CompilerNotSupported);
        }

        if (Rules.Any() == false)
        {
            // There is no rules.
            return Item;
        }

        JsonNode ModuleRule = Rules.First()!;

        CppModuleDescriptor Parse(JsonNode Element)
        {
            bool bIsInterface = (bool?)Element["is-interface"] ?? false;
            string? LookupMethod = (string?)Element["lookup-method"];

            return new CppModuleDescriptor()
            {
                InterfaceName = (string)Element["logical-name"]!,
                SourcePath = (string?)Element["source-path"],
                IsInterface = bIsInterface,
                LookupMethod = LookupMethod switch
                {
                    "include-angle" => ModuleLookupMethod.Angle,
                    "include-quote" => ModuleLookupMethod.Quote,
                    _ => null
                }
            };
        }

        CppModuleDescriptor[] Provides = ((JsonArray?)ModuleRule["provides"] ?? new()).Select(p => Parse(p!)).ToArray();
        CppModuleDescriptor[] Requires = ((JsonArray?)ModuleRule["requires"] ?? new()).Select(p => Parse(p!)).ToArray();

        Item.Cache.bScanDependenciesCache = true;
        Item.Cache.Provide = Provides.FirstOrDefault();
        Item.Cache.Requires = Requires;
        return Item;
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
