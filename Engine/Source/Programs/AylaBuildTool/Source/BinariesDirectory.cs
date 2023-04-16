// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Source;

public readonly struct BinariesDirectory
{
    private readonly string _Root;

    public string Root
    {
        get => _Root;
        init
        {
            _Root = value;
            Win64 = Path.Combine(_Root, "Win64");
            DotNET = Path.Combine(_Root, "DotNET");
        }
    }

    public string Win64 { get; private init; }

    public string DotNET { get; private init; }

    public void GenerateDirectoriesRecursive()
    {
        if (Directory.Exists(Root) == false)
        {
            Directory.CreateDirectory(Root);
        }

        if (Directory.Exists(Win64) == false)
        {
            Directory.CreateDirectory(Win64);
        }

        if (Directory.Exists(DotNET) == false)
        {
            Directory.CreateDirectory(DotNET);
        }
    }

    public void Cleanup()
    {
        string[] Targets = new string[]
        {
            Win64
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
