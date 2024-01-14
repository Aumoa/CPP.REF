// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.IO;

namespace AE.Source;

public struct IntermediateDirectory
{
    private readonly DirectoryReference _Root;

    public DirectoryReference Root
    {
        get => _Root;
        init
        {
            _Root = value;
            Includes = _Root.GetChild("Includes");
            Shaders = _Root.GetChild("Shaders");
            ProjectFiles = _Root.GetChild("ProjectFiles");
            CSharp = _Root.GetChild("CSharp");
            Build = _Root.GetChild("Build");
            Makefiles = _Root.GetChild("Makefiles");
            Assemblies = _Root.GetChild("Assemblies");
            Unused = _Root.GetChild("Unused");
        }
    }

    public DirectoryReference Includes { get; private init; }
    
    public DirectoryReference Shaders { get; private init; }

    public DirectoryReference ProjectFiles { get; private init; }

    public DirectoryReference CSharp { get; private init; }

    public DirectoryReference Build { get; private init; }

    public DirectoryReference Makefiles { get; private init; }

    public DirectoryReference Assemblies { get; private init; }

    public DirectoryReference Unused { get; private init; }

    public void GenerateDirectoriesRecursive()
    {
        Root.Create();
        Includes.Create();
        ProjectFiles.Create();
        CSharp.Create();
        Build.Create();
        Makefiles.Create();
        Assemblies.Create();
        Unused.Create();
    }

    public void Cleanup()
    {
        Includes.Clear();
        CSharp.Clear();
        Build.Clear();
        Makefiles.Clear();
        Assemblies.Clear();
    }

    public override string ToString()
    {
        return Root;
    }

    public DirectoryReference BuildsOut(BuildConfiguration config, string moduleName)
    {
        return Build.GetHierarchy(config.Platform.ToString(), config.Configuration.ToString(), moduleName);
    }
}
