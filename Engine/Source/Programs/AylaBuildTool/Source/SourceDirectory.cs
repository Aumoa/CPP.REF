// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Source;

public readonly struct SourceDirectory
{
    private readonly string _Root;

    public string Root
    {
        get => _Root;
        init
        {
            _Root = value;
            Programs = Path.Combine(_Root, "Programs");
        }
    }

    public string Programs { get; private init; }

    public void GenerateDirectoriesRecusrive()
    {
        if (Directory.Exists(Root) == false)
        {
            Directory.CreateDirectory(Root);
        }

        if (Directory.Exists(Programs) == false)
        {
            Directory.CreateDirectory(Programs);
        }
    }

    public override string ToString()
    {
        return Root;
    }
}
