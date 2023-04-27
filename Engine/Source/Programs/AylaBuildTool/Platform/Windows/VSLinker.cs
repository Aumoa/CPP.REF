// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Data;
using System.Diagnostics;
using System.Text;

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

        IEnumerable<string> LinkLibraries = Module.AdditionalLibraries.Select(p => $"\"{p}\"");
        LinkLibraries = LinkLibraries.Concat(Module.DependModules.Select(p => $"\"{p}.lib\""));
        string LinkLibrary = string.Join(' ', LinkLibraries);
        LinkLibrary = $"/DYNAMICBASE {LinkLibrary}";

        string Subsystem = string.Empty;

        if (Module.TargetType == TargetType.Module)
        {
            BinaryOut = Path.ChangeExtension(BinaryOut, ".dll");
            Subsystem = "/DLL";
        }
        else if (Module.TargetType == TargetType.ConsoleApplication)
        {
            BinaryOut = Path.ChangeExtension(BinaryOut, ".exe");
            Subsystem = "/SUBSYSTEM:CONSOLE";
        }
        else if (Module.TargetType == TargetType.SlateApplication)
        {
            BinaryOut = Path.ChangeExtension(BinaryOut, ".exe");
            Subsystem = "/SUBSYSTEM:WINDOWS";
        }

        PSI.Arguments = $"/nologo /DEBUG {LinkLibrary} {Subsystem} /MACHINE:X64 /OUT:\"{BinaryOut}\" /LIBPATH:\"{LibraryPath}\" /LIBPATH:\"{OutputDir}\" /LTCG \"{BuildDir}\\*.obj\"";

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
            throw new TerminateException(KnownErrorCode.CompileError, Report);
        }

        return Stdout;
    }
}
