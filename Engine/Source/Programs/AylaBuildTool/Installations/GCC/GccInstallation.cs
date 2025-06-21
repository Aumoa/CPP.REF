
namespace AylaEngine;

internal class GccInstallation : Installation
{
    public override ValueTask<CppCompiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<CppCompiler>(new GccCompiler(targetInfo));
    }

    public override ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<Linker>(new GccLinker(targetInfo));
    }
}