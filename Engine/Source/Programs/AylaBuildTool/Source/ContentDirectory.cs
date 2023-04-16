// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Source;

public readonly struct ContentDirectory
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
