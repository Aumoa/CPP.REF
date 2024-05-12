// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public readonly struct SourceDirectory
{
    private readonly DirectoryReference _Root;

    public DirectoryReference Root
    {
        get => _Root;
        init
        {
            _Root = value;
            Programs = _Root.GetChild("Programs");
        }
    }

    public DirectoryReference Programs { get; private init; }

    public void GenerateDirectoriesRecusrive()
    {
        Root.Create();
        Programs.Create();
    }

    public override string ToString()
    {
        return Root;
    }
}
