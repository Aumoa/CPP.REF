// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.IO;

namespace AE.Source;

public readonly struct BinariesDirectory
{
    private readonly DirectoryReference _Root;

    public DirectoryReference Root
    {
        get => _Root;
        init
        {
            _Root = value;
            DotNET = Root.GetChild("DotNET");
            Interop = Root.GetChild("Interop");
            CSharp = Root.GetChild("CSharp");
        }
    }

    public DirectoryReference DotNET { get; private init; }

    public DirectoryReference Interop { get; private init; }

    public DirectoryReference CSharp { get; private init; }

    public void GenerateDirectoriesRecursive()
    {
        Root.Create();
        DotNET.Create();
        Interop.Create();
        CSharp.Create();
    }

    public void Cleanup()
    {
        Interop.Clear();
        CSharp.Clear();
    }

    public override string ToString()
    {
        return Root;
    }

    public DirectoryReference BinariesOut(TargetPlatform platform, Configuration configuration)
    {
        return _Root.GetHierarchy(platform.ToString(), configuration.ToString());
    }

    public DirectoryReference BinariesOut(BuildConfiguration buildConfig) => BinariesOut(buildConfig.Platform, buildConfig.Configuration);
}
