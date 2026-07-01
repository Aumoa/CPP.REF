namespace AylaEngine;

internal class GccCompiler : UnixCompiler
{
    public GccCompiler(Installation installation, TargetInfo targetInfo) : base(installation, targetInfo)
    {
    }

    protected override string GetPchOutputFilePath(CppPchSettings pchSettings)
    {
        return pchSettings.SourceFilePath + ".gch";
    }

    protected override string[] GetCreatePchArguments(CppPchSettings pchSettings)
    {
        return ["-x c++-header"];
    }

    protected override string[] GetUsePchArguments(CppPchSettings pchSettings)
    {
        return
        [
            $"-include \"{pchSettings.SourceFilePath}\"",
            "-fpch-deps"
        ];
    }
}