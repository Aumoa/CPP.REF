namespace AylaEngine;

internal class GccLinker : UnixLinker
{
    public GccLinker(Installation installation, TargetInfo targetInfo) : base(installation, targetInfo)
    {
    }

    protected override ValueTask<string[]> ConfigureCommandsAsync(bool isShared, CancellationToken cancellationToken)
    {
        List<string> commands = [];

        if (isShared)
        {
            commands.Add("-shared");
        }

        if (m_TargetInfo.Config is Configuration.Development or Configuration.Shipping)
        {
            commands.Add("-ffunction-sections");
            commands.Add("-fdata-sections");
            commands.Add("-Wl,--gc-sections");
        }

        return ValueTask.FromResult(commands.ToArray());
    }

    protected override ValueTask<string[]> ConfigureLibrariesAsync(IEnumerable<string> libraries, CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<string[]>(libraries
            .Append("stdc++")
            .Append("stdc++exp")
            .Select(l => $"-l\"{l}\"")
            .ToArray());
    }
}