﻿// Copyright 2020-2022 Aumoa.lib. All right reserved.

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

    private static Dictionary<Compiler, List<VisualStudioInstallation>> CachedVisualStudioInstallations = new();

    public static VisualStudioInstallation[] FindVisualStudioInstallations(Compiler InCompiler)
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
                        Compiler.VisualStudio2022 => new Version(17, 0),
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

    private Dictionary<Architecture, string> CompilerPaths = new();

    public override string FindCCompilerPath(Architecture TargetArchitecture)
    {
        if (CompilerPaths.TryGetValue(TargetArchitecture, out string? CompilerPath) == false)
        {
            string MSVCToolsetsPath = Path.Combine(BaseDirectory, "VC", "Tools", "MSVC");
            if (Directory.Exists(MSVCToolsetsPath) == false)
            {
                throw new InvalidOperationException(CoreStrings.Errors.InvalidToolChainInstallation);
            }

            List<Version> CompilerVersions = new();
            foreach (var VersionDir in Directory.GetDirectories(MSVCToolsetsPath))
            {
                CompilerVersions.Add(Version.Parse(Path.GetFileName(VersionDir)!));
            }

            string HostArchitecture = BuildHostPlatform.Current.Platform.Architecture switch
            {
                Architecture.x64 => "Hostx64",
                _ => throw new NotSupportedException(CoreStrings.Errors.NotSupportedBuildHostPlatform)
            };

            Version LatestVersion = CompilerVersions.OrderByDescending(p => p).First();
            CompilerPath = Path.Combine(MSVCToolsetsPath, LatestVersion.ToString(), "bin", HostArchitecture, TargetArchitecture.ToString(), "cl.exe");
            if (File.Exists(CompilerPath) == false)
            {
                throw new InvalidOperationException(CoreStrings.Errors.InvalidToolChainInstallation);
            }

            CompilerPaths.Add(TargetArchitecture, CompilerPath);
        }

        return CompilerPath;
    }
}
