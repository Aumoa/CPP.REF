// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.BuildSettings;
using AE.CompilerServices;
using AE.Exceptions;
using AE.Projects;
using AE.Rules;

namespace AE.Platform.Linux;

public class GccLinker : Linker
{
    private readonly LinuxToolChain ToolChain;
    private readonly string CompilerPath;

    public GccLinker(LinuxToolChain ToolChain, string CompilerPath)
    {
        this.ToolChain = ToolChain;
        this.CompilerPath = CompilerPath;
    }

    private void GenerateBuildInformation(ModuleInformation Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string Subsystem)
    {
        var Config = Target.Config;
        if (Config.Platform == TargetPlatform.Linux)
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
        BuildDir = Path.Combine(BuildDir, Config.Configuration.ToString(), Module.Name);
        Subsystem = string.Empty;

        if (Module.TargetType == ModuleRules.ModuleType.Library)
        {
            string Filename = Path.GetFileNameWithoutExtension(BinaryOut);
            BinaryOut = Path.ChangeExtension(Path.Combine(Path.GetDirectoryName(BinaryOut)!, "lib" + Filename), ".so");
            Subsystem = "-shared";
        }
        else if (Module.TargetType == ModuleRules.ModuleType.ConsoleApplication)
        {
            BinaryOut = Path.Combine(Path.GetDirectoryName(BinaryOut)!, Path.GetFileNameWithoutExtension(BinaryOut));
            Subsystem = "";
        }
        else
        {
            throw new TerminateException(KnownErrorCode.NotSupportedType, CoreStrings.Errors.NotSupportedType("TargetType", "SlateApplication"));
        }
    }

    public override async Task<string> LinkAsync(ModuleInformation Module, CancellationToken SToken = default)
    {
        GenerateBuildInformation(Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string Subsystem);

        ProcessStartInfo PSI = new(CompilerPath);
        PSI.RedirectStandardOutput = true;
        PSI.RedirectStandardError = true;
        PSI.Environment["LDFLAGS"] = "-Wl,--copy-dt-needed-entries";

        IEnumerable<string> Links = Module.PrivateAdditionalLibraries.Concat(Module.DependModules);
        IEnumerable<string> LinkLibraries = Links.Select(p => $"-l\"{p}\"");
        string LinkLibrary = $"{string.Join(' ', LinkLibraries)} ";

        IEnumerable<string> Inputs = Directory.GetFiles(BuildDir, "*.o").Select(p => Path.GetRelativePath(BuildDir, p));
        string InputsStr = string.Join(' ', Inputs);

        PSI.WorkingDirectory = BuildDir;
        PSI.UseShellExecute = false;
        PSI.Arguments = $"{Subsystem} -o \"{BinaryOut}\" {InputsStr} -lc -lgcc -lstdc++ -lm {LinkLibrary} -L/usr/lib/gcc/x86_64-linux-gnu/13 -L\"{LibraryDir}\"";
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
            Console.Error.WriteLine(PSI.Arguments);
            throw new TerminateException(KnownErrorCode.CompileError, Report);
        }

        return $"Linking: {BinaryOut}";
    }

    public override Task<bool> TryCheckOutputsAsync(ModuleInformation Module, CancellationToken SToken = default)
    {
        GenerateBuildInformation(Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string Subsystem);
        return Task.FromResult(File.Exists(BinaryOut));
    }
}
