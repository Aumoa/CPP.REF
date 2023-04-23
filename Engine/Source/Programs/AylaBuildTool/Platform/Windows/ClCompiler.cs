// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;
using System.Text;

using AE.BuildSettings;
using AE.Exceptions;
using AE.Misc;
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

    public override async Task<string> CompileAsync(MakefileCompile Item, TargetRules Rule, CancellationToken SToken = default)
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
        PSI.StandardOutputEncoding = KnownEncodings.EUCKR;

        string Output = Item.Output;
        if (Directory.Exists(Output) == false)
        {
            Directory.CreateDirectory(Output);
        }
        PSI.Arguments = $"/nologo /c /Fo\"{Output}\\\\\" /std:c++20 /EHsc /showIncludes";

        // Appending additional arguments.
        string IncludePaths = string.Join(' ', Item.IncludePaths.Select(p => $"/I\"{p}\""));
        PSI.Arguments += $" {IncludePaths}";
        List<string> Macros = new();
        Macros.AddRange(GenerateBuildMacros(Rule).Select(p => $"/D\"{p.Item1}\"=\"{p.Item2}\""));
        Macros.AddRange(Item.AdditionalMacros.Select(p => $"/D\"{p}\""));
        Macros.AddRange(Item.DependModules.Select(p => $"/D\"{p.ToUpper()}_API\"=\"__declspec(dllimport)\""));
        Macros.Add($"/D\"{Item.ModuleName.ToUpper()}_API\"=\"__declspec(dllexport)\"");

        string AdditionalMacros = string.Join(' ', Macros);
        PSI.Arguments += $" {AdditionalMacros}";

        // Include source.
        PSI.Arguments += $" {Item.SourceCode}";

        Process? P = Process.Start(PSI);
        if (P == null)
        {
            throw new InvalidOperationException("Internal error.");
        }

        string Stdout = await P.StandardOutput.ReadToEndAsync(SToken);
        string Stderr = await P.StandardError.ReadToEndAsync(SToken);
        await P.WaitForExitAsync(SToken);
        Stdout += await P.StandardOutput.ReadToEndAsync(SToken);
        Stderr += await P.StandardError.ReadToEndAsync(SToken);

        string[] Lines = Stdout.Split('\n', StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries);
        Dictionary<string, SourceCodeHash> SourceCodeHashes = new();
        List<Task> Tasks = new();
        Stdout = string.Empty;

        foreach (var Line in Lines)
        {
            string? Location = null;
            if (Line.StartsWith("참고: 포함 파일: "))
            {
                Location = Line["참고: 포함 파일: ".Length..];
            }
            else if (Line.StartsWith("Note: including file: "))
            {
                Location = Line["Note: including file: ".Length..];
            }
            else
            {
                Stdout += $"{Line}\n";
                continue;
            }

            Tasks.Add(SourceCodeHash.GenerateHashAsync(Location, SToken).ContinueWith(p =>
            {
                lock (SourceCodeHashes)
                {
                    SourceCodeHashes[Location] = p.Result;
                }
            }));
        }

        await Task.WhenAll(Tasks);

        if (P.ExitCode != 0)
        {
            string Report = string.Empty;
            if (P.ExitCode == 1 || P.ExitCode == 2)
            {
                Report += Stdout;
            }
            throw new TerminateException(9, Report.Replace("{", "{{").Replace("}", "}}"));
        }

        Item.IncludeHashes = SourceCodeHashes;
        return Lines[0];
    }
}
