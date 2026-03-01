namespace AylaEngine;

internal class ClangLinker : UnixLinker
{
    public ClangLinker(Installation installation, TargetInfo targetInfo) : base(installation, targetInfo)
    {
    }

    protected override ValueTask<string[]> ConfigureCommandsForSharedLibraryAsync(CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<string[]>(["-dynamiclib"]);
    }

    protected override ValueTask<string[]> ConfigureLibrariesAsync(IEnumerable<string> libraries, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<string[]>(libraries
            .Select(l => $"-l\"{l}\"")
            .ToArray());
    }
}