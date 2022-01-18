// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

using System.IO;

public abstract class FileSystemReference
{
    public FileSystemReference(string InPath)
    {
        FullPath = Path.GetFullPath(InPath);
        Name = Path.GetFileNameWithoutExtension(FullPath);
        Root = Path.GetPathRoot(FullPath);
    }

    public string GetRelativePath(FileSystemReference InReference)
    {
        return Path.GetRelativePath(FullPath, InReference.FullPath);
    }

    public DirectoryReference GetParent()
    {
        return new(Directory.GetParent(FullPath).FullName);
    }

    public override string ToString()
    {
        return FullPath;
    }

    public string Name { get; }

    public string FullPath { get; }

    public string Root { get; }
}