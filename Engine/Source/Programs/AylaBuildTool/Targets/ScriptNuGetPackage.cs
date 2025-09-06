namespace AylaEngine;

/// <summary>
/// Represents a NuGet package used in scripts, including its ID and version.
/// </summary>
/// <param name="Id"> The unique identifier of the NuGet package. </param>
/// <param name="Version"> The version of the NuGet package. </param>
public record struct ScriptNuGetPackage(string Id, Version Version)
{
    /// <summary>
    /// Parses a string in the format "PackageId, Version=1.2.3" into a <see cref="ScriptNuGetPackage"/> instance.
    /// </summary>
    /// <param name="value"> A string containing the package ID and version, e.g., "PackageId, Version=1.2.3". </param>
    /// <returns> A <see cref="ScriptNuGetPackage"/> instance parsed from the input string. </returns>
    /// <exception cref="FormatException"> Thrown when the input string is not in the expected format or contains invalid data. </exception>
    public static ScriptNuGetPackage Parse(string value)
    {
        var parts = value.Split(',');
        if (parts.Length != 2)
        {
            throw new FormatException("Invalid format. Expected format: 'PackageId, Version'");
        }
        var id = parts[0].Trim();
        var versionString = parts[1].Trim().Replace("Version=", "", StringComparison.OrdinalIgnoreCase);
        if (string.IsNullOrEmpty(id))
        {
            throw new FormatException("PackageId cannot be null or empty.");
        }
        if (!Version.TryParse(versionString, out var version))
        {
            throw new FormatException("Invalid version format.");
        }
        return new ScriptNuGetPackage(id, version);
    }

    public static implicit operator ScriptNuGetPackage(string value) => Parse(value);
}
