// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Exceptions;
using System.Diagnostics;
using System.Threading;

using AE.Projects;

namespace AE.CompilerServices;

public static class ReflectionCodeGen
{
    public static async Task GenerateReflectionCodesAsync(IEnumerable<ModuleInformation> targetModules, CancellationToken cancellationToken = default)
    {
        var reflectionModules = targetModules
            .Where(p => p.DependModules.Contains("CoreAObject"));

        if (reflectionModules.Any())
        {
            var psi = new ProcessStartInfo();

            if (Environment.OSVersion.Platform == PlatformID.Unix)
            {
                string baseDir = Path.Combine(Global.EngineDirectory.Binaries.Linux, "Shipping");
                psi.Environment["LD_LIBRARY_PATH"] = baseDir;
                psi.FileName = Path.Combine(baseDir, "AylaHeaderTool");
            }
            else if (Environment.OSVersion.Platform == PlatformID.Win32NT)
            {
                psi.FileName = Global.EngineDirectory.Binaries.Win64;
                psi.FileName = Path.Combine(psi.FileName, "Shipping", "AylaHeaderTool.exe");
            }
            else
            {
                throw new TerminateException(KnownErrorCode.NotSupportedBuildHostPlatform, CoreStrings.Errors.NotSupportedBuildHostPlatform);
            }

            psi.WorkingDirectory = Global.EngineDirectory.Root;

            List<Task> tasks = new();
            foreach (var reflectionModule in reflectionModules)
            {
                psi.Arguments = $"""
                -Source "{reflectionModule.SourcePath}" -Includes "{reflectionModule.GeneratedIncludePath}" -PackageName "{reflectionModule.Name}" -CSharp "{reflectionModule.CSharpPath}.Interop"
                """;

                if (Directory.Exists(reflectionModule.GeneratedIncludePath) == false)
                {
                    Directory.CreateDirectory(reflectionModule.GeneratedIncludePath);
                }

                Console.WriteLine("  Running AylaHeaderTool \"{0}\"", reflectionModule.SourcePath);
                Process? p = Process.Start(psi);
                Debug.Assert(p != null);

                tasks.Add(p.WaitForExitAsync(cancellationToken).ContinueWith(task =>
                {
                    if (task.IsCanceled || task.IsFaulted)
                    {
                        throw task.Exception!;
                    }

                    if (p.ExitCode != 0)
                    {
                        throw new TerminateException(KnownErrorCode.CompileError, "Internal Reflection Code Error");
                    }
                }));
            }

            await Task.WhenAll(tasks);
        }
    }
}
