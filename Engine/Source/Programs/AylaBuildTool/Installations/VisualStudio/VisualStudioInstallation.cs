using System.Diagnostics;
using System.Text.Json.Nodes;

namespace AylaEngine;

/// <summary>
/// Represents a Visual Studio installation and provides methods to interact with its components,
/// such as compilers, linkers, and Windows SDKs. This class is responsible for discovering
/// installed Visual Studio products and Windows Kits, and for spawning compiler and linker
/// instances for Windows build targets.
/// </summary>
internal class VisualStudioInstallation : Installation
{
    public enum License
    {
        BuildTool,
        Community,
        Professional,
        Enterprise
    }

    public enum VSVersion
    {
        _2022,
        _2026
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

    private const string KitRoot = "C:\\Program Files (x86)\\Windows Kits\\10";
    private const string VSRoot = "C:\\Program Files\\Microsoft Visual Studio";

    private static Version? s_WindowsKitVersion;
    private static Product[] s_Products;

    static VisualStudioInstallation()
    {
        List<Version> kitVersions = [];
        if (Directory.Exists(Path.Combine(KitRoot, "Include")) == false)
        {
            s_Products = Array.Empty<Product>();
            return;
        }

        foreach (var fullPath in Directory.GetDirectories(Path.Combine(KitRoot, "Include")))
        {
            string versionStr = Path.GetFileName(fullPath);
            if (Version.TryParse(versionStr, out var version) == false)
            {
                continue;
            }

            if (Directory.Exists(Path.Combine(KitRoot, "bin", versionStr)) == false)
            {
                continue;
            }

            if (Directory.Exists(Path.Combine(KitRoot, "Lib", versionStr)) == false)
            {
                continue;
            }

            kitVersions.Add(version);
        }

        kitVersions.Sort((l, r) => r.CompareTo(l));
        s_WindowsKitVersion = kitVersions.First();

        var products = DiscoverProducts();
        products.Sort(CompareProducts);
        s_Products = products.ToArray();
        if (s_Products.Length == 0)
        {
            s_WindowsKitVersion = null;
        }
    }

    private static List<Product> DiscoverProducts()
    {
        var products = DiscoverProductsFromKnownFolders();
        if (products.Count > 0)
        {
            return products;
        }

        return DiscoverProductsWithVswhere();
    }

    private static List<Product> DiscoverProductsWithVswhere()
    {
        List<Product> products = [];

        var programFilesX86 = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86);
        if (string.IsNullOrWhiteSpace(programFilesX86))
        {
            return products;
        }

        var vswhere = Path.Combine(programFilesX86, "Microsoft Visual Studio", "Installer", "vswhere.exe");
        if (File.Exists(vswhere) == false)
        {
            return products;
        }

        try
        {
            var startInfo = new ProcessStartInfo
            {
                FileName = vswhere,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                CreateNoWindow = true
            };

            startInfo.ArgumentList.Add("-all");
            startInfo.ArgumentList.Add("-prerelease");
            startInfo.ArgumentList.Add("-products");
            startInfo.ArgumentList.Add("*");
            startInfo.ArgumentList.Add("-format");
            startInfo.ArgumentList.Add("json");

            using var process = Process.Start(startInfo);
            if (process == null)
            {
                return products;
            }

            var output = process.StandardOutput.ReadToEnd();
            process.StandardError.ReadToEnd();
            process.WaitForExit();

            if (process.ExitCode != 0 || string.IsNullOrWhiteSpace(output))
            {
                return products;
            }

            var instances = JsonNode.Parse(output)?.AsArray();
            if (instances == null)
            {
                return products;
            }

            foreach (var instance in instances)
            {
                var installationDirectory = ReadString(instance?["installationPath"]);
                if (string.IsNullOrWhiteSpace(installationDirectory))
                {
                    continue;
                }

                var catalog = instance?["catalog"];
                var visualStudioVersion = ParseVisualStudioVersion(
                    ReadString(catalog?["productLineVersion"]),
                    ReadString(catalog?["productLine"]),
                    ReadString(instance?["installationVersion"]),
                    Path.GetFileName(Path.GetDirectoryName(installationDirectory)),
                    Path.GetFileName(installationDirectory));
                if (visualStudioVersion == null)
                {
                    continue;
                }

                var license = ParseLicense(ReadString(instance?["productId"]), installationDirectory);
                AddProductsFromInstallationDirectory(products, license, visualStudioVersion.Value, installationDirectory);
            }
        }
        catch
        {
            products.Clear();
        }

        return products;
    }

    private static List<Product> DiscoverProductsFromKnownFolders()
    {
        List<Product> products = [];
        if (Directory.Exists(VSRoot) == false)
        {
            return products;
        }

        foreach (var visualStudioVersionDirectory in Directory.GetDirectories(VSRoot))
        {
            var visualStudioVersion = ParseVisualStudioVersion(Path.GetFileName(visualStudioVersionDirectory));
            if (visualStudioVersion == null)
            {
                continue;
            }

            foreach (var licenseDirectory in Directory.GetDirectories(visualStudioVersionDirectory))
            {
                var license = ParseLicense(null, licenseDirectory);
                AddProductsFromInstallationDirectory(products, license, visualStudioVersion.Value, licenseDirectory);
            }

            AddProductsFromInstallationDirectory(products, License.BuildTool, visualStudioVersion.Value, visualStudioVersionDirectory);
        }

        return products;
    }

    private static void AddProductsFromInstallationDirectory(List<Product> products, License license, VSVersion visualStudioVersion, string installationDirectory)
    {
        var msvc = Path.Combine(installationDirectory, "VC", "Tools", "MSVC");
        if (Directory.Exists(msvc) == false)
        {
            return;
        }

        foreach (var versionDir in Directory.GetDirectories(msvc))
        {
            var versionStr = Path.GetFileName(versionDir);
            if (Version.TryParse(versionStr, out var version) == false)
            {
                continue;
            }

            products.Add(new Product
            {
                License = license,
                VisualStudioVersion = visualStudioVersion,
                CompilerVersion = version,
                Directory = versionDir
            });
        }
    }

    private static int CompareProducts(Product l, Product r)
    {
        var visualStudioVersionCompare = GetVisualStudioVersionRank(r.VisualStudioVersion).CompareTo(GetVisualStudioVersionRank(l.VisualStudioVersion));
        if (visualStudioVersionCompare != 0)
        {
            return visualStudioVersionCompare;
        }

        return r.CompilerVersion.CompareTo(l.CompilerVersion);
    }

    private static int GetVisualStudioVersionRank(VSVersion value) => value switch
    {
        VSVersion._2026 => 2026,
        VSVersion._2022 => 2022,
        _ => 0
    };

    private static License ParseLicense(string? productId, string installationDirectory)
    {
        if (productId?.EndsWith(".BuildTools", StringComparison.OrdinalIgnoreCase) == true)
        {
            return License.BuildTool;
        }

        foreach (var license in Enum.GetValues<License>())
        {
            if (productId?.EndsWith("." + license, StringComparison.OrdinalIgnoreCase) == true)
            {
                return license;
            }
        }

        if (Enum.TryParse<License>(Path.GetFileName(installationDirectory), true, out var parsedLicense))
        {
            return parsedLicense;
        }

        return License.Community;
    }

    private static VSVersion? ParseVisualStudioVersion(params string?[] values)
    {
        foreach (var value in values)
        {
            if (string.IsNullOrWhiteSpace(value))
            {
                continue;
            }

            if (value.Equals("2026", StringComparison.OrdinalIgnoreCase) ||
                value.Equals("18", StringComparison.OrdinalIgnoreCase) ||
                value.Equals("Dev18", StringComparison.OrdinalIgnoreCase) ||
                value.StartsWith("18.", StringComparison.OrdinalIgnoreCase))
            {
                return VSVersion._2026;
            }

            if (value.Equals("2022", StringComparison.OrdinalIgnoreCase) ||
                value.Equals("17", StringComparison.OrdinalIgnoreCase) ||
                value.Equals("Dev17", StringComparison.OrdinalIgnoreCase) ||
                value.StartsWith("17.", StringComparison.OrdinalIgnoreCase))
            {
                return VSVersion._2022;
            }
        }

        return null;
    }

    private static string? ReadString(JsonNode? node) => node?.GetValue<string>();

    public static VSVersion? DetectedVersion => s_Products.Length > 0 ? s_Products[0].VisualStudioVersion : null;

    public override ValueTask<string> GetCompilerPath(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        if (targetInfo.Platform.Group != PlatformGroup.Windows)
        {
            Console.Error.WriteLine("Non-windows build target not supported.");
            throw TerminateException.User();
        }

        var product = s_Products[0];
        return ValueTask.FromResult(product.GetClCompiler(targetInfo.Platform.Architecture));
    }

    public override ValueTask<string> GetIntelliSenseMode(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        switch (targetInfo.Platform.Architecture)
        {
            case Architecture.X64:
                return ValueTask.FromResult("windows-msvc-x64");
        }

        Console.Error.WriteLine("Architecture({0}) not support.", targetInfo.Platform.Architecture);
        throw TerminateException.NotSupport();
    }

    public override ValueTask<CppCompiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        if (targetInfo.Platform.Group != PlatformGroup.Windows)
        {
            Console.Error.WriteLine("Non-windows build target not supported.");
            throw TerminateException.User();
        }

        var product = s_Products[0];
        return ValueTask.FromResult<CppCompiler>(new ClCompiler(this, targetInfo, product));
    }

    public override ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        if (targetInfo.Platform.Group != PlatformGroup.Windows)
        {
            Console.Error.WriteLine("Non-windows build target not supported.");
            throw TerminateException.User();
        }

        var product = s_Products[0];
        return ValueTask.FromResult<Linker>(new MSLinker(this, targetInfo, product));
    }

    public override async ValueTask<string[]> ParseDependenciesAsync(string depsFileName, CancellationToken cancellationToken)
    {
        var json = await File.ReadAllTextAsync(depsFileName, cancellationToken);
        var includes = JsonNode.Parse(json)?["Data"]?["Includes"]?.AsArray();
        return includes!.Select(p => p!.GetValue<string>()).ToArray();
    }

    public override string OutputFileName(string projectName, ModuleType moduleType)
    {
        return projectName + moduleType switch
        {
            ModuleType.Library => ".dll",
            ModuleType.Game => ".dll",
            ModuleType.Application => ".exe",
            ModuleType.Console => ".exe",
            _ => string.Empty
        };
    }

    public static IEnumerable<string> GatherWindowsKitInclude()
    {
        if (s_WindowsKitVersion == null)
        {
            Console.Error.WriteLine("Non-windows build target not supported.");
            throw TerminateException.User();
        }

        string versionString = s_WindowsKitVersion.ToString(4);
        string @base = Path.Combine(KitRoot, "Include", versionString);
        yield return Path.Combine(@base, "ucrt");
        yield return Path.Combine(@base, "um");
        yield return Path.Combine(@base, "shared");
        yield return Path.Combine(@base, "winrt");
    }

    public static IEnumerable<string> GatherWindowsKitSharedLibrary(Architecture arch)
    {
        if (s_WindowsKitVersion == null)
        {
            Console.Error.WriteLine("Non-windows build target not supported.");
            throw TerminateException.User();
        }

        string versionString = s_WindowsKitVersion.ToString(4);
        string architectureString = arch switch
        {
            Architecture.X64 => "x64",
            _ => throw TerminateException.Internal()
        };

        string @base = Path.Combine(KitRoot, "Lib", versionString);
        yield return Path.Combine(@base, "um", architectureString);
        yield return Path.Combine(@base, "ucrt", architectureString);
    }
}
