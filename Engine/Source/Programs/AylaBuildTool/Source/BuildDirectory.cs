// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public readonly struct BuildDirectory
{
    private readonly string _Root;

    public string Root
    {
        get => _Root;
        init
        {
            _Root = value;
        }
    }

    public void GenerateDirectoriesRecursive()
    {
        if (Directory.Exists(Root) == false)
        {
            Directory.CreateDirectory(Root);
        }
    }

    public override string ToString()
    {
        return Root;
    }
}
