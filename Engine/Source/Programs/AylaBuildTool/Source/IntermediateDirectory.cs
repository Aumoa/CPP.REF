// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Source;

public struct IntermediateDirectory
{
    private readonly string _Root;

    public string Root
    {
        get => _Root;
        init
        {
            _Root = value;
            Includes = Path.Combine(_Root, "Includes");
            ProjectFiles = Path.Combine(_Root, "ProjectFiles");
            Build = Path.Combine(_Root, "Build");
            StdBuild = Path.Combine(Build, "Std");
            Makefiles = Path.Combine(_Root, "Makefiles");
            Unused = Path.Combine(_Root, "Unused");
        }
    }

    public string Includes { get; private init; }

    public string ProjectFiles { get; private init; }

    public string Build { get; private init; }

    public string StdBuild { get; private init; }

    public string Makefiles { get; private init; }

    public string Unused { get; private init; }

    public void GenerateDirectoriesRecursive()
    {
        if (Directory.Exists(Root) == false)
        {
            Directory.CreateDirectory(Root);
        }

        if (Directory.Exists(Includes) == false)
        {
            Directory.CreateDirectory(Includes);
        }

        if (Directory.Exists(ProjectFiles) == false)
        {
            Directory.CreateDirectory(ProjectFiles);
        }

        if (Directory.Exists(Build) == false)
        {
            Directory.CreateDirectory(Build);
        }

        if (Directory.Exists(StdBuild) == false)
        {
            Directory.CreateDirectory(StdBuild);
        }

        if (Directory.Exists(Makefiles) == false)
        {
            Directory.CreateDirectory(Makefiles);
        }

        if (Directory.Exists(Unused) == false)
        {
            Directory.CreateDirectory(Unused);
        }
    }

    public void Cleanup()
    {
        string[] Targets = new string[]
        {
            Makefiles
        };

        foreach (var Target in Targets)
        {
            if (Directory.Exists(Target))
            {
                Directory.Delete(Target, true);
                Directory.CreateDirectory(Target);
            }
        }
    }

    public override string ToString()
    {
        return Root;
    }
}
