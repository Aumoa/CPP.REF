namespace AylaEngine;

public class ScriptRule : IReadOnlyScriptRule
{
    public bool Enabled { get; set; }

    public List<ScriptNuGetPackage> NuGetPackages { get; } = [];

    IReadOnlyList<ScriptNuGetPackage> IReadOnlyScriptRule.NuGetPackages => NuGetPackages;
}
