
namespace AylaEngine;

internal class GccInstallation : Installation
{
    public override ValueTask<string> GetCompilerPath(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult("/usr/bin/gcc");
    }

    public override ValueTask<string> GetIntelliSenseMode(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        switch (targetInfo.Platform.Group)
        {
            case PlatformGroup.Windows:
                switch (targetInfo.Platform.Architecture)
                {
                    case Architecture.X64:
                        return ValueTask.FromResult("windows-gcc-x64");
                }
                break;
            case PlatformGroup.Linux:
                switch (targetInfo.Platform.Architecture)
                {
                    case Architecture.X64:
                        return ValueTask.FromResult("linux-gcc-x64");
                }
                break;
        }

        Console.Error.WriteLine("Architecture({0}) not support.", targetInfo.Platform.Architecture);
        throw TerminateException.NotSupport();
    }

    public override ValueTask<CppCompiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<CppCompiler>(new GccCompiler(this, targetInfo));
    }

    public override ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<Linker>(new GccLinker(targetInfo));
    }

    public override async ValueTask<string[]> ParseDependenciesAsync(string depsFileName, CancellationToken cancellationToken)
    {
        var plain = await File.ReadAllTextAsync(depsFileName, cancellationToken);
        var lines = plain.Split('\n').Select(p => p.Trim(['\\', '\r'])).ToArray();
        if (lines.Length <= 2)
        {
            return Array.Empty<string>();
        }

        return lines[2..].Select(p => p.Trim()).Where(p => string.IsNullOrEmpty(p) == false).ToArray();
    }
}