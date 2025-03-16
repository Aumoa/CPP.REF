using Spectre.Console;

namespace AylaEngine;

internal class VisualStudioInstallation : Installation
{
    public enum License
    {
        Community
    }

    public enum VSVersion
    {
        _2022
    }

    public readonly struct Product
    {
        public required License License { get; init; }
        public required VSVersion VisualStudioVersion { get; init; }
        public required Version CompilerVersion { get; init; }
        public required string Directory { get; init; }
        
        public string GetClCompiler(Architecture arch)
        {
            return Path.Combine(Directory, "bin", "Hostx64", arch.ToString().ToLower(), "cl.exe");
        }

        public string GetIncludeDirectory()
        {
            return Path.Combine(Directory, "include");
        }

        public override string ToString() => Directory;
    }

    private const string Root = "C:\\Program Files\\Microsoft Visual Studio";

    private static IEnumerable<Product> FindProducts()
    {
        if (Directory.Exists(Root) == false)
        {
            AnsiConsole.MarkupLine("[red]No Visual Studio installation founds.[/]");
            yield break;
        }

        foreach (var visualStudioVersionDirectory in Directory.GetDirectories(Root))
        {
            VSVersion visualStudioVersion;
            switch (Path.GetFileName(visualStudioVersionDirectory))
            {
                case "2022":
                    visualStudioVersion = VSVersion._2022;
                    break;
                default:
                    continue;
            }

            foreach (var licenseDirectory in Directory.GetDirectories(visualStudioVersionDirectory))
            {
                if (Enum.TryParse<License>(Path.GetFileName(licenseDirectory), out var license) == false)
                {
                    continue;
                }

                var msvc = Path.Combine(licenseDirectory, "VC", "Tools", "MSVC");
                if (Directory.Exists(msvc) == false)
                {
                    continue;
                }

                foreach (var versionDir in Directory.GetDirectories(msvc))
                {
                    var versionStr = Path.GetFileName(versionDir);
                    if (Version.TryParse(versionStr, out var version) == false)
                    {
                        continue;
                    }

                    yield return new Product
                    {
                        License = license,
                        VisualStudioVersion = visualStudioVersion,
                        CompilerVersion = version,
                        Directory = versionDir
                    };
                }
            }
        }
    }
}
