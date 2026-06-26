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

    protected override string GetPchOutputFilePath(CppPchSettings pchSettings)
    {
        return pchSettings.PchFilePath;
    }

    protected override string[] GetCreatePchArguments(CppPchSettings pchSettings)
    {
        return ["-x c++-header"];
    }

    protected override string[] GetUsePchArguments(CppPchSettings pchSettings)
    {
        return [$"-include-pch \"{pchSettings.PchFilePath}\""];
    }
}
