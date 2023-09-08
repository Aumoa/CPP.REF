// Copyright 2020-2022 Aumoa.lib. All right reserved.

using System.Diagnostics;

using AE.BuildSettings;
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

    private void GenerateBuildInformation(TargetRules Rule, ModuleInformation Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string Subsystem)
    {
        var Config = Rule.Target.BuildConfiguration;
        if (Config.Platform == TargetPlatform.Linux)
        {
            OutputDir = Path.Combine(Module.ProjectDir.Binaries.Linux, Config.Configuration.ToString());
            BuildDir = Path.Combine(Module.ProjectDir.Intermediate.Build, "Linux");
            LibraryDir = Path.Combine(Global.EngineDirectory.Binaries.Linux, Config.Configuration.ToString());

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

        if (Module.TargetType == TargetType.Module)
        {
            string Filename = Path.GetFileNameWithoutExtension(BinaryOut);
            BinaryOut = Path.ChangeExtension(Path.Combine(Path.GetDirectoryName(BinaryOut)!, "lib" + Filename), ".so");
            Subsystem = "-shared";
        }
        else if (Module.TargetType == TargetType.ConsoleApplication)
        {
            BinaryOut = Path.Combine(Path.GetDirectoryName(BinaryOut)!, Path.GetFileNameWithoutExtension(BinaryOut));
            Subsystem = "";
        }
        else
        {
            throw new TerminateException(KnownErrorCode.NotSupportedType, CoreStrings.Errors.NotSupportedType("TargetType", "SlateApplication"));
        }
    }

    public override async Task<string> LinkAsync(TargetRules Rule, ModuleInformation Module, CancellationToken SToken = default)
    {
        GenerateBuildInformation(Rule, Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string Subsystem);

        ProcessStartInfo PSI = new(CompilerPath);
        PSI.RedirectStandardOutput = true;
        PSI.RedirectStandardError = true;
        PSI.Environment["LDFLAGS"] = "-Wl,--copy-dt-needed-entries";

        IEnumerable<string> Links = Module.AdditionalLibraries.Concat(Module.DependModules);
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

    public override Task<bool> TryCheckOutputsAsync(TargetRules Rule, ModuleInformation Module, CancellationToken SToken = default)
    {
        GenerateBuildInformation(Rule, Module, out string OutputDir, out string BuildDir, out string LibraryDir, out string BinaryOut, out string Subsystem);
        return Task.FromResult(File.Exists(BinaryOut));
    }
}
