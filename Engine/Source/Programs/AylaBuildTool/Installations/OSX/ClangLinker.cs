namespace AylaEngine;

internal class ClangLinker : UnixLinker
{
    public ClangLinker(Installation installation, TargetInfo targetInfo) : base(installation, targetInfo)
    {
    }

    protected override ValueTask<string[]> ConfigureCommandsAsync(bool isShared, CancellationToken cancellationToken)
    {
        List<string> commands = [];

        if (isShared)
        {
            commands.Add("-dynamiclib");
        }

        if (m_TargetInfo.Config is Configuration.Development or Configuration.Shipping)
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