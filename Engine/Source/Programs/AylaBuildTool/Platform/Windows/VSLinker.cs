// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Data;
using System.Diagnostics;

using AE.BuildSettings;
using AE.Exceptions;
using AE.Misc;
using AE.Projects;
using AE.Rules;

namespace AE.Platform.Windows;

public class VSLinker : Linker
{
    private readonly string LinkExe;
    private readonly VisualStudioInstallation ToolChain;

    public VSLinker(string LinkExe, VisualStudioInstallation ToolChain)
    {
        this.LinkExe = LinkExe;
        this.ToolChain = ToolChain;
    }

    public override async Task<string> LinkAsync(TargetRules Rule, ModuleDependenciesResolver.ModuleDependencyCache Module, CancellationToken SToken = default)
    {
        string LinkExe = this.LinkExe;
        var Config = Rule.Target.BuildConfiguration;
        foreach (var ExecutablePath in ToolChain.GetRequiredExecutablePaths(Config.Platform.Architecture))
        {
            string Cur = Path.Combine(ExecutablePath, this.LinkExe);
            if (File.Exists(Cur))
            {
                LinkExe = Cur;
                break;
            }
        }

        string OutputDir;
        string BuildDir;
        string LibraryPath;
        if (Config.Platform == TargetPlatform.Win64)
        {
            OutputDir = Module.ProjectDir.Binaries.Win64;
            BuildDir = Path.Combine(Module.ProjectDir.Intermediate.Build, "Win64");
            LibraryPath = Global.EngineDirectory.Binaries.Win64;
        }
        else
        {
            throw new InvalidOperationException("Internal error.");
        }

        string BinaryOut = Path.Combine(OutputDir, Module.Name);
        BuildDir = Path.Combine(BuildDir, Config.Configuration.ToString(), Module.Name);

        ProcessStartInfo PSI = new(LinkExe);
        string[] Paths = ToolChain.GetRequiredExecutablePaths(Config.Platform.Architecture);
        PSI.Environment["PATH"] = string.Join(';', Paths);
        string[] Libs = ToolChain.GetRequiredLibraryPaths(Config.Platform.Architecture);
        PSI.Environment["LIB"] = string.Join(';', Libs);
        PSI.RedirectStandardOutput = true;
        PSI.RedirectStandardError = true;
        PSI.StandardOutputEncoding = KnownEncodings.EUCKR;

        BinaryOut = Path.ChangeExtension(BinaryOut, ".dll");
        IEnumerable<string> DynamicBases = Module.AdditionalLibraries.Select(p => $"\"{p}\"");
        DynamicBases = DynamicBases.Concat(Module.DependModules.Select(p => $"\"{p}.lib\""));
        string DynamicBase = string.Join(' ', DynamicBases);
        PSI.Arguments = $"/nologo /DLL /DEBUG /DYNAMICBASE {DynamicBase} /OUT:\"{BinaryOut}\" /LIBPATH:\"{LibraryPath}\" /LIBPATH:\"{OutputDir}\" \"{BuildDir}\\*.obj\"";

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

        if (P.ExitCode != 0)
        {
            string Report = Stdout + Stderr;
            throw new TerminateException(9, Report.Replace("{", "{{").Replace("}", "}}"));
        }

        return Stdout;
    }
}
