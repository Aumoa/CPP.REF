namespace AylaEngine;

internal class ClangCompiler : UnixCompiler
{
    public ClangCompiler(Installation installation, TargetInfo targetInfo) : base(installation, targetInfo)
    {
    }

    protected override ValueTask<string[]> GetCompilerArgumentsAsync(CancellationToken cancellationToken)
    {
        return ValueTask.FromResult<string[]>(["-fexperimental-library", "-Wswitch", "-Wswitch-enum"]);
    }
}
