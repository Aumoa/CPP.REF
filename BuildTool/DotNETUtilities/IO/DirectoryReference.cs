// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace DotNETUtilities;
using System.IO;

public class DirectoryReference : FileSystemReference
{
    public DirectoryReference(string InPath) : base(InPath)
    {
    }

    public void CreateIfNotExists(bool bRecursive = false)
    {
        if (!IsExist)
        {
            if (bRecursive)
            {
                GetParent()?.CreateIfNotExists(bRecursive);
            }

            Directory.CreateDirectory(FullPath);
        }
    }

    public DirectoryReference Move(string ChangeDirectoryPath)
    {
        return new(Path.Combine(FullPath, ChangeDirectoryPath));
    }

    public FileReference[] GetCurrentFiles()
    {
        return ConvertFilePaths(Directory.GetFiles(FullPath, "*.*"));
    }

    public FileReference[] GetAllFiles()
    {
        try
        {
            return ConvertFilePaths(Directory.GetFiles(FullPath, "*.*", SearchOption.AllDirectories));
        }
        catch (Exception)
        {
            return Array.Empty<FileReference>();
        }
    }

    public DirectoryReference[] GetCurrentDirectories()
    {
        return ConvertDirectoryPaths(Directory.GetDirectories(FullPath, "*", SearchOption.TopDirectoryOnly));
    }

    public bool IsExist
    {
        get => Directory.Exists(FullPath);
    }

    private FileReference[] ConvertFilePaths(string[] InPaths)
    {
        if (InPaths is null || InPaths.Length == 0)
        {
            return new FileReference[0];
        }
        var References = new FileReference[InPaths.Length];
        for (int i = 0; i < InPaths.Length; ++i)
        {
            References[i] = new(InPaths[i]);
        }

        return References;
    }

    private DirectoryReference[] ConvertDirectoryPaths(string[] InPaths)
    {
        if (InPaths is null || InPaths.Length == 0)
        {
            return new DirectoryReference[0];
        }
        var References = new DirectoryReference[InPaths.Length];
        for (int i = 0; i < InPaths.Length; ++i)
        {
            References[i] = new(InPaths[i]);
        }

        return References;
    }
}