namespace AylaEngine;

public interface IReadOnlyScriptRule
{
    bool Enabled { get; }

    IReadOnlyList<ScriptNuGetPackage> NuGetPackages { get; }
}
