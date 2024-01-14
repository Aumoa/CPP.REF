// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Data;
using System.Diagnostics;

using AE.BuildSettings;
using AE.CompilerServices;
using AE.Exceptions;
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

    private void GenerateBuildInformation(ModuleInformation Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string PdbOut, out string Subsystem)
    {
        var Config = Target.Config;
        if (Config.Platform == TargetPlatform.Win64)
        {
            OutputDir = Module.ProjectDir.Binaries.BinariesOut(Config);
            BuildDir = Module.ProjectDir.Intermediate.BuildsOut(Config, Module.Name);
            LibraryDir = Global.EngineDirectory.Binaries.BinariesOut(Config);

            if (Directory.Exists(OutputDir) == false)
            {
                Directory.CreateDirectory(OutputDir);
            }
        }
        else
        {
            throw new InvalidOperationException("Internal error.");
        }

        BinaryOut = Path.Combine(OutputDir, Module.Name);
        PdbOut = Path.Combine(OutputDir, Module.Name + ".pdb");

        if (Module.TargetType == ModuleRules.ModuleType.Library)
        {
            BinaryOut = Path.ChangeExtension(BinaryOut, ".dll");
            Subsystem = "/DLL";
        }
        else if (Module.TargetType == ModuleRules.ModuleType.ConsoleApplication)
        {
            BinaryOut = Path.ChangeExtension(BinaryOut, ".exe");
            Subsystem = "/SUBSYSTEM:CONSOLE";
        }
        else if (Module.TargetType == ModuleRules.ModuleType.Application)
        {
            BinaryOut = Path.ChangeExtension(BinaryOut, ".exe");
            Subsystem = "/SUBSYSTEM:WINDOWS";
        }
        else
        {
            throw new InvalidOperationException("Internal error.");
        }
    }

    public override async Task<string> LinkAsync(ModuleInformation Module, CancellationToken SToken = default)
    {
        string LinkExe = this.LinkExe;
        var Config = Target.Config;
        foreach (var ExecutablePath in ToolChain.GetRequiredExecutablePaths(Config.Platform.Architecture))
        {
            string Cur = Path.Combine(ExecutablePath, this.LinkExe);
            if (File.Exists(Cur))
            {
                LinkExe = Cur;
                break;
            }
        }

        GenerateBuildInformation(Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string PdbOut, out string Subsystem);

        ProcessStartInfo PSI = new(LinkExe);
        string[] Paths = ToolChain.GetRequiredExecutablePaths(Config.Platform.Architecture);
        PSI.Environment["PATH"] = string.Join(';', Paths);
        string[] Libs = ToolChain.GetRequiredLibraryPaths(Config.Platform.Architecture);
        PSI.Environment["LIB"] = string.Join(';', Libs);
        PSI.RedirectStandardOutput = true;
        PSI.RedirectStandardError = true;

        IEnumerable<string> LinkLibraries = Module.PrivateAdditionalLibraries.Select(p => $"\"{p}\"");
        LinkLibraries = LinkLibraries.Concat(Module.DependModules.Select(p => $"\"{p}.lib\""));
        string LinkLibrary = string.Join(' ', LinkLibraries);
        LinkLibrary = $"/DYNAMICBASE {LinkLibrary}";

        PSI.Arguments = $"/nologo /DEBUG {LinkLibrary} {Subsystem} /MACHINE:X64 /OUT:\"{BinaryOut}\" /LIBPATH:\"{LibraryDir}\" /LIBPATH:\"{OutputDir}\" \"{BuildDir}\\*.cpp.obj\" \"{BuildDir}\\*.ixx.obj\"";

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

        Stdout = Stdout.Trim();
        if (Stdout.Length != 0)
        {
            Stdout += "\n";
        }
        Stdout += $"{BinaryOut}";

        return Stdout;
    }

    public override Task<bool> TryCheckOutputsAsync(ModuleInformation Module, CancellationToken SToken)
    {
        GenerateBuildInformation(Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string PdbOut, out string Subsystem);
        return Task.FromResult(File.Exists(BinaryOut) && File.Exists(PdbOut));
    }
}
