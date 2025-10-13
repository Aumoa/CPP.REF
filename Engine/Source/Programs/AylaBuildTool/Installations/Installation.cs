using System.Runtime.InteropServices;

namespace AylaEngine;

internal abstract class Installation
{
    public abstract ValueTask<string> GetCompilerPath(TargetInfo targetInfo, CancellationToken cancellationToken);
    public abstract ValueTask<string> GetIntelliSenseMode(TargetInfo targetInfo, CancellationToken cancellationToken);

    public abstract ValueTask<CppCompiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken);
    public abstract ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken);

    public abstract ValueTask<string[]> ParseDependenciesAsync(string depsFileName, CancellationToken cancellationToken);
    public abstract string OutputFileName(string projectName, ModuleType moduleType);

    public static Installation CreateDefaultInstallation()
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            return new VisualStudioInstallation();
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            return new GccInstallation();
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            return new GccInstallation();
        }
        else
        {
            Console.Error.WriteLine("Installation: Not supported platform. {0}", RuntimeInformation.OSDescription);
            throw TerminateException.Internal();
        }
    }
}
