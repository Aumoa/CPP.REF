// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;
using AE.IO;
using AE.Misc;

using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace AE.Source;

public readonly struct ProjectDirectory : IEquatable<ProjectDirectory>
{
    private readonly DirectoryReference _Root;

    public DirectoryReference Root
    {
        get => _Root;
        init
        {
            _Root = value;
            Name = Root.Name;
            Source = new() { Root = Root.GetChild("Source") };
            Shaders = _Root.GetChild("Shaders");
            Intermediate = new() { Root = Root.GetChild("Intermediate") };
            Content = new() { Root = Root.GetChild("Content") };
            Binaries = new() { Root = Root.GetChild("Binaries") };
            Build = new() { Root = Root.GetChild("Build") };
        }
    }

    public string Name { get; private init; }

    public SourceDirectory Source { get; private init; }

    public DirectoryReference Shaders { get; private init; }

    public IntermediateDirectory Intermediate { get; private init; }

    public ContentDirectory Content { get; init; }

    public BinariesDirectory Binaries { get; init; }
    
    public BuildDirectory Build { get; private init; }

    public override bool Equals(object? obj)
    {
        if (obj is ProjectDirectory pd)
        {
            return this == pd;
        }

        return false;
    }

    public bool Equals(ProjectDirectory rhs)
    {
        return this == rhs;
    }

    public override int GetHashCode() => _Root.GetHashCode();

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
        Intermediate.Cleanup();
        Binaries.Cleanup();
    }

    public override string ToString()
    {
        return Root;
    }

    public static bool operator ==(ProjectDirectory lhs, ProjectDirectory rhs)
    {
        return lhs._Root == rhs._Root;
    }

    public static bool operator !=(ProjectDirectory lhs, ProjectDirectory rhs)
    {
        return lhs._Root != rhs._Root;
    }

    public static implicit operator DirectoryReference(ProjectDirectory value) => value._Root;

    public static implicit operator ProjectDirectory(DirectoryReference value) => new ProjectDirectory { Root = value };
}
