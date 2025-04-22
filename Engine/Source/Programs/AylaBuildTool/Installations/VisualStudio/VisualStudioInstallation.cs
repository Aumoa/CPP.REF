namespace AylaEngine;

internal class VisualStudioInstallation : Installation
{
    public enum License
    {
        Community,
        Professional
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

    private const string KitRoot = "C:\\Program Files (x86)\\Windows Kits\\10";
    private const string VSRoot = "C:\\Program Files\\Microsoft Visual Studio";

    private static Version? s_WindowsKitVersion;
    private static Product[] s_Products;

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

    public override ValueTask<CppCompiler> SpawnCompilerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        if (targetInfo.Platform.Group != PlatformGroup.Windows)
        {
            Console.Error.WriteLine("Non-windows build target not supported.");
            throw TerminateException.User();
        }

        var product = s_Products[0];
        return ValueTask.FromResult<CppCompiler>(new ClCompiler(targetInfo, product));
    }

    public override ValueTask<Linker> SpawnLinkerAsync(TargetInfo targetInfo, CancellationToken cancellationToken)
    {
        if (targetInfo.Platform.Group != PlatformGroup.Windows)
        {
            Console.Error.WriteLine("Non-windows build target not supported.");
            throw TerminateException.User();
        }

        var product = s_Products[0];
        return ValueTask.FromResult<Linker>(new MSLinker(targetInfo, product));
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
