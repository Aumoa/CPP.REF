using System.Runtime.InteropServices;

namespace AylaEngine;

internal abstract class Installation
{
    public abstract ValueTask<CppCompiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken);
    public abstract ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken);

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
        else
        {
            throw new PlatformNotSupportedException();
        }
    }
}
