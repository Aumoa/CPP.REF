namespace AylaEngine;

internal class ClangLinker : UnixLinker
{
    public ClangLinker(Installation installation, TargetInfo targetInfo) : base(installation, targetInfo)
    {
    }

    protected override ValueTask<string[]> ConfigureCommandsAsync(ModuleRulesResolver module, bool isShared, CancellationToken cancellationToken)
    {
        List<string> commands = [];

        if (isShared)
        {
            commands.Add("-dynamiclib");
        }

        if (module.BuildProfile.IsOptimized)
        {
            commands.Add("-Wl,-dead_strip");
        }

        return ValueTask.FromResult<string[]>(commands.ToArray());
    }

    protected override ValueTask<string[]> ConfigureLibrariesAsync(IEnumerable<string> libraries, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<string[]>(libraries
            .Select(l => $"-l\"{l}\"")
            .ToArray());
    }
}
