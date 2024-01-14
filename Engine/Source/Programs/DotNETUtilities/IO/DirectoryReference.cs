// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Misc;

namespace AE.IO;

public class DirectoryReference : FileSystemReference
{
    public DirectoryReference(string path) : base(path)
    {
    }

    public override DirectoryReference ToCurrentDirectoryBased()
    {
        return new DirectoryReference(Path.Combine(Environment.CurrentDirectory, Value));
    }

    public void Create()
    {
        Directory.CreateDirectory(Value);
    }

    public void Delete(bool recursive = false)
    {
        if (IsExists)
        {
            Directory.Delete(Value, recursive);
        }
    }

    public void Clear()
    {
        bool exists = IsExists;
        Delete(true);
        if (exists)
        {
            Create();
        }
    }

    public DirectoryReference[] GetDirectories(bool recursive = false) => IsExists
        ? Directory.GetDirectories(Value, "*", recursive
            ? SearchOption.AllDirectories
            : SearchOption.TopDirectoryOnly)
            .Select(p => new DirectoryReference(p))
            .ToArray()
        : Array.Empty<DirectoryReference>();

    public FileReference[] GetFiles(string searchPattern = "*.*", bool recursive = false) => IsExists
        ? Directory.GetFiles(Value, searchPattern, recursive
            ? SearchOption.AllDirectories
            : SearchOption.TopDirectoryOnly)
            .Select(p => new FileReference(p))
            .ToArray()
        : Array.Empty<FileReference>();

    public DirectoryReference GetChild(string name) => Path.Combine(TrimPath(Value), name);

    public FileReference GetFile(string name) => Path.Combine(TrimPath(Value), name);

    public DirectoryReference GetHierarchy(params string[] hierarchy) => GetChild(Path.Combine(hierarchy));

    public DirectoryReference GetParent()
    {
        var path = TrimPath(Value).Replace(Path.AltDirectorySeparatorChar, Path.DirectorySeparatorChar);
        int indexOf = path.LastIndexOf(Path.DirectorySeparatorChar);
        if (path.IsValidIndex(indexOf) == false)
        {
            throw new InvalidOperationException();
        }

        string parentValue = path[..indexOf];
        return parentValue;
    }

    public override bool IsExists => Directory.Exists(Value);

    public override DirectoryReference Absolute => new(AbsolutePath);

    public static implicit operator DirectoryReference(string path) => new DirectoryReference(path);
}
