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
            Linux = Path.Combine(_Root, "Linux");
            DotNET = Path.Combine(_Root, "DotNET");
            Interop = Path.Combine(_Root, "Interop");
            CSharp = Path.Combine(_Root, "CSharp");
        }
    }

    public string Win64 { get; private init; }

    public string Linux { get; private init; }

    public string DotNET { get; private init; }

    public string Interop { get; private init; }

    public string CSharp { get; private init; }

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

        if (Directory.Exists(Linux) == false)
        {
            Directory.CreateDirectory(Linux);
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
            Win64,
            Linux,
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
