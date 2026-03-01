using System.Runtime.InteropServices;

namespace AylaEngine;

internal class OSXInstallation : UnixInstallation
{
    public override ValueTask<string> GetCompilerPath(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult("/usr/bin/clang++");
    }

    public override ValueTask<string> GetIntelliSenseMode(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        switch (targetInfo.Platform.Architecture)
        {
            case Architecture.Arm64:
                return ValueTask.FromResult("macos-clang-arm64");
        }

        Console.Error.WriteLine("Architecture({0}) not support.", targetInfo.Platform.Architecture);
        throw TerminateException.NotSupport();
    }

    public override ValueTask<CppCompiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<CppCompiler>(new ClangCompiler(this, targetInfo));
    }

    public override ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<Linker>(new ClangLinker(this, targetInfo));
    }

    public override string OutputFileName(string projectName, ModuleType moduleType)
    {
        const string kSharedLibExt = "dylib";
        return moduleType switch
        {
            ModuleType.Library => $"lib{projectName}.{kSharedLibExt}",
            ModuleType.Game => $"lib{projectName}.{kSharedLibExt}",
            ModuleType.Application => projectName,
            ModuleType.Console => projectName,
            ModuleType.ThirdParty => projectName,
            _ => string.Empty
        };
    }
}
