namespace AylaEngine;

internal class GccCompiler : UnixCompiler
{
    public GccCompiler(Installation installation, TargetInfo targetInfo) : base(installation, targetInfo)
    {
    }
}