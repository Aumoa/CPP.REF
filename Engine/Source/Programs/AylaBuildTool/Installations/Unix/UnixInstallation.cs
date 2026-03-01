using System.Runtime.InteropServices;

namespace AylaEngine;

internal abstract class UnixInstallation : Installation
{
    public override async ValueTask<string[]> ParseDependenciesAsync(string depsFileName, CancellationToken cancellationToken)
    {
        var plain = await File.ReadAllTextAsync(depsFileName, cancellationToken);
        var lines = plain.Split('\n').Select(p => p.Trim(['\\', '\r'])).ToArray();
        if (lines.Length <= 2)
        {
            return Array.Empty<string>();
        }

        return lines[2..].Select(p => p.Trim()).Where(p => string.IsNullOrEmpty(p) == false).ToArray();
    }
}
