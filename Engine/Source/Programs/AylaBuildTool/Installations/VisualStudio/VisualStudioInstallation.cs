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
        Enterprise,
        Preview
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

    public static Product? BestProduct => s_Products.Length > 0 ? s_Products[0] : null;

    static VisualStudioInstallation()
    {
        if (Directory.Exists(VSRoot) == false)
        {
            s_Products = Array.Empty<Product>();
            return;
        }

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

        List<Product> products = [];
        foreach (var visualStudioVersionDirectory in Directory.GetDirectories(VSRoot))
        {
            VSVersion visualStudioVersion;
            switch (Path.GetFileName(visualStudioVersionDirectory))
            {
                case "2022":
                    visualStudioVersion = VSVersion._2022;
                    break;
                case "2026":
                    visualStudioVersion = VSVersion._2026;
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

                CheckFolder(license, licenseDirectory);
            }

            CheckFolder(License.BuildTool, visualStudioVersionDirectory);
            continue;

            void CheckFolder(License license, string folder)
            {
                var msvc = Path.Combine(folder, "VC", "Tools", "MSVC");
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
        }

        products.Sort((l, r) => r.CompilerVersion.CompareTo(l.CompilerVersion));
        s_Products = products.ToArray();
        if (s_Products.Length == 0)
        {
            s_WindowsKitVersion = null;
        }
    }

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
