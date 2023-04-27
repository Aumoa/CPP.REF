// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.Misc;

namespace AE.Source;

public readonly struct ProjectDirectory
{
    private readonly string _Root;

    public string Root
    {
        get => _Root;
        init
        {
            _Root = value;
            Name = Path.GetFileNameWithoutExtension(value);
            Source = new() { Root = Path.Combine(Root, "Source") };
            Intermediate = new() { Root = Path.Combine(Root, "Intermediate") };
            Content = new() { Root = Path.Combine(Root, "Content") };
            Binaries = new() { Root = Path.Combine(Root, "Binaries") };
            Build = new() { Root = Path.Combine(Root, "Build") };
        }
    }

    public string Name { get; private init; }

    public SourceDirectory Source { get; private init; }

    public IntermediateDirectory Intermediate { get; private init; }

    public ContentDirectory Content { get; init; }

    public BinariesDirectory Binaries { get; init; }
    
    public BuildDirectory Build { get; private init; }

    public void GenerateDirectoriesRecursive()
    {
        if (Directory.Exists(Root) == false)
        {
            Directory.CreateDirectory(Root);
        }

        Source.GenerateDirectoriesRecusrive();
        Intermediate.GenerateDirectoriesRecursive();
        Content.GenerateDirectoriesRecursive();
        Binaries.GenerateDirectoriesRecursive();
        Build.GenerateDirectoriesRecursive();
    }

    public void Cleanup()
    {
        DirectoryExtensions.Cleanup(Intermediate.Build);
        DirectoryExtensions.Cleanup(Intermediate.Includes);
        DirectoryExtensions.Cleanup(Intermediate.Makefiles);
        DirectoryExtensions.Cleanup(Binaries.Win64);
    }

    public override string ToString()
    {
        return Root;
    }

    public string GenerateIntermediateOutput(BuildConfiguration Config, string ModuleName)
    {
        return Path.Combine(Intermediate.Build, Config.Platform.ToString(), Config.Configuration.ToString(), ModuleName);
    }
}
