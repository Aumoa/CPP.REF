// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.System;

using Microsoft.VisualStudio.Setup.Configuration;

namespace AE.Platform.Windows;

public class VisualStudioInstallation : ToolChainInstallation
{
    private VisualStudioInstallation(string InBaseDirectory, Version InInstallVersion)
    {
        BaseDirectory = InBaseDirectory;
        InstallVersion = InInstallVersion;
    }

    public override string BaseDirectory { get; }

    public override Version InstallVersion { get; }

    public bool bIsPreview { get; init; }

    private static readonly Dictionary<CompilerVersion, List<VisualStudioInstallation>> CachedVisualStudioInstallations = new();

    public static VisualStudioInstallation[] FindVisualStudioInstallations(CompilerVersion InCompiler)
    {
        if (CachedVisualStudioInstallations.TryGetValue(InCompiler, out var Installations) == false)
        {
            Installations = new List<VisualStudioInstallation>();
            SetupConfiguration Setup = new();
            IEnumSetupInstances Enumerator = Setup.EnumAllInstances();

            try
            {
                while (true)
                {
                    ISetupInstance[] Instances = new ISetupInstance[1];
                    Enumerator.Next(1, Instances, out int NumFetched);
                    if (NumFetched == 0)
                    {
                        break;
                    }

                    ISetupInstance2 Instance = (ISetupInstance2)Instances[0];
                    bool bIsLocalState = (Instance.GetState() & InstanceState.Local) == InstanceState.Local;
                    if (!bIsLocalState)
                    {
                        continue;
                    }

                    var InstallVersion = Version.Parse(Instance.GetInstallationVersion());
                    Version CurrentVersion = InCompiler switch
                    {
                        CompilerVersion.VisualStudio2022 => new Version(17, 0),
                        _ => throw new ArgumentException(CoreStrings.Errors.CompilerNotSupported, nameof(InCompiler))
                    };

                    if (InstallVersion < CurrentVersion || InstallVersion > new Version(CurrentVersion.Major, 99))
                    {
                        continue;
                    }

                    ISetupInstanceCatalog? Catalog = Instance as ISetupInstanceCatalog;
                    var Installation = new VisualStudioInstallation(Instance.GetInstallationPath(), InstallVersion)
                    {
                        bIsPreview = Catalog?.IsPrerelease() == true
                    };

                    Installations.Add(Installation);
                }
            }
            catch
            {
                Console.Error.WriteLine("An exception occurred while search Visual Studio Compiler for {0}.", InCompiler);
            }

            CachedVisualStudioInstallations.Add(InCompiler, Installations);
        }

        return Installations.ToArray();
    }

    private struct CompilerAndLibraryPath
    {
        public string CompilerPath { get; set; }

        public string[] LibraryPath { get; set; }

        public string[] IncludePath { get; set; }
    }

    private readonly Dictionary<Architecture, CompilerAndLibraryPath> CompilerPaths = new();

    private CompilerAndLibraryPath CacheVisualStudioPath(Architecture TargetArchitecture)
    {
        if (CompilerPaths.TryGetValue(TargetArchitecture, out CompilerAndLibraryPath PathSet) == false)
        {
            string ArchPath = TargetArchitecture switch
            {
                Architecture.x64 => "x64",
                _ => throw new InvalidOperationException(string.Format(CoreStrings.Errors.NotSupportedArchitecture(TargetArchitecture.ToString())))
            };

            string CompilerPath;
            string MSVCToolsetsPath = Path.Combine(BaseDirectory, "VC", "Tools", "MSVC");
            if (Directory.Exists(MSVCToolsetsPath) == false)
            {
                throw new InvalidOperationException(CoreStrings.Errors.InvalidToolChainInstallation);
            }

            List<string> CompilerVersions = new();
            foreach (var VersionDir in Directory.GetDirectories(MSVCToolsetsPath))
            {
                CompilerVersions.Add(Path.GetFileName(VersionDir)!);
            }

            string HostArchitecture = BuildHostPlatform.Current.Platform.Architecture switch
            {
                Architecture.x64 => "Hostx64",
                _ => throw new NotSupportedException(CoreStrings.Errors.NotSupportedBuildHostPlatform)
            };

            Version LatestVersion = SelectVersion(CompilerVersions);
            CompilerPath = Path.Combine(MSVCToolsetsPath, LatestVersion.ToString(), "bin", HostArchitecture, ArchPath);
            if (File.Exists(Path.Combine(CompilerPath, "cl.exe")) == false)
            {
                throw new InvalidOperationException(CoreStrings.Errors.InvalidToolChainInstallation);
            }

            PathSet.CompilerPath = CompilerPath;
            PathSet.LibraryPath = new[] { Path.Combine(MSVCToolsetsPath, LatestVersion.ToString(), "lib", ArchPath) };
            PathSet.IncludePath = new[] { Path.Combine(MSVCToolsetsPath, LatestVersion.ToString(), "include") };
            CompilerPaths.Add(TargetArchitecture, PathSet);
        }

        return PathSet;
    }

    private readonly Dictionary<Architecture, CompilerAndLibraryPath> CachedWindowsKitVersion = new();

    private CompilerAndLibraryPath CacheWindowsKitVersion(Architecture InArchitecture)
    {
        if (CachedWindowsKitVersion.TryGetValue(InArchitecture, out CompilerAndLibraryPath CachedVersion) == false)
        {
            string ArchPath = InArchitecture switch
            {
                Architecture.x64 => "x64",
                _ => throw new InvalidOperationException(string.Format(CoreStrings.Errors.NotSupportedArchitecture(InArchitecture.ToString())))
            };

            List<string> CandidateVersions = new();

            const string BaseLibraryDirectory = "C:\\Program Files (x86)\\Windows Kits\\10\\Lib";
            foreach (var VersionDirectory in Directory.GetDirectories(BaseLibraryDirectory))
            {
                string Location = Path.Combine(VersionDirectory, "ucrt", ArchPath, "ucrt.lib");
                if (File.Exists(Location) == false)
                {
                    continue;
                }

                CandidateVersions.Add(Path.GetFileName(VersionDirectory));
            }

            const string BaseCompilerDirectory = "C:\\Program Files (x86)\\Windows Kits\\10\\bin";
            string[] VersionDirectories = CandidateVersions.ToArray();
            CandidateVersions.Clear();
            foreach (var VersionDirectory in VersionDirectories)
            {
                string Location = Path.Combine(BaseCompilerDirectory, VersionDirectory, ArchPath, "rc.exe");
                if (File.Exists(Location) == false)
                {
                    continue;
                }

                CandidateVersions.Add(VersionDirectory);
            }

            if (CandidateVersions.Count == 0)
            {
                throw new InvalidOperationException(CoreStrings.Errors.InvalidToolChainInstallation);
            }

            const string BaseIncludeDirectory = "C:\\Program Files (x86)\\Windows Kits\\10\\Include";

            string CurrentVersion = SelectVersion(CandidateVersions).ToString();
            CachedVersion.CompilerPath = Path.Combine(BaseCompilerDirectory, CurrentVersion, ArchPath);
            CachedVersion.LibraryPath = new[]
            {
                Path.Combine(BaseLibraryDirectory, CurrentVersion, "um", ArchPath),
                Path.Combine(BaseLibraryDirectory, CurrentVersion, "ucrt", ArchPath)
            };
            CachedVersion.IncludePath = new[]
            {
                Path.Combine(BaseIncludeDirectory, CurrentVersion, "ucrt"),
                Path.Combine(BaseIncludeDirectory, CurrentVersion, "um"),
                Path.Combine(BaseIncludeDirectory, CurrentVersion, "shared"),
                Path.Combine(BaseIncludeDirectory, CurrentVersion, "winrt")
            };

            CachedWindowsKitVersion.Add(InArchitecture, CachedVersion);
        }

        return CachedVersion;
    }

    public override string[] GetRequiredExecutablePaths(Architecture TargetArchitecture)
    {
        CompilerAndLibraryPath VSSet = CacheVisualStudioPath(TargetArchitecture);
        CompilerAndLibraryPath WindowsSet = CacheWindowsKitVersion(TargetArchitecture);
        return new[] { VSSet.CompilerPath, WindowsSet.CompilerPath, "C:\\Program Files\\dotnet" };
    }

    public override string[] GetRequiredLibraryPaths(Architecture TargetArchitecture)
    {
        CompilerAndLibraryPath VSSet = CacheVisualStudioPath(TargetArchitecture);
        CompilerAndLibraryPath WindowsSet = CacheWindowsKitVersion(TargetArchitecture);
        return VSSet.LibraryPath.Concat(WindowsSet.LibraryPath).ToArray();
    }

    public override string[] GetRequiredIncludePaths(Architecture TargetArchitecture)
    {
        CompilerAndLibraryPath VSSet = CacheVisualStudioPath(TargetArchitecture);
        CompilerAndLibraryPath WindowsSet = CacheWindowsKitVersion(TargetArchitecture);
        return VSSet.IncludePath.Concat(WindowsSet.IncludePath).ToArray();
    }

    public override Compiler SpawnCompiler()
    {
        return new ClCompiler("cl.exe", this);
    }

    public override Linker SpawnLinker()
    {
        return new VSLinker("link.exe", this);
    }

    public override string GetOutputExtension()
    {
        return ".obj";
    }

    public override string GetShaderCompilerDirectory(Architecture TargetArchitecture)
    {
        var WindowsSet = CacheWindowsKitVersion(TargetArchitecture);
        return WindowsSet.CompilerPath;
    }

    public override string DotNET => $"C:\\Program Files\\dotnet\\dotnet.exe";

    private Version SelectVersion(IEnumerable<string> Versions, Func<Version, bool> Predicate)
    {
        return Versions.Select(Version.Parse).Where(Predicate).OrderByDescending(p => p).First();
    }

    private Version SelectVersion(IEnumerable<string> Versions)
    {
        return SelectVersion(Versions, p => true);
    }
}
