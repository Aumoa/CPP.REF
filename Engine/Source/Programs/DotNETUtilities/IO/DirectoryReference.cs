// Copyright 2020-2023 Aumoa.lib. All right reserved.

namespace AE.IO;

public class DirectoryReference : FileSystemReference
{
    public DirectoryReference(string path) : base(path)
    {
    }

    public string[] GetDirectories(bool recursive = false) => IsExists
        ? Directory.GetDirectories(Value, "*", recursive
            ? SearchOption.AllDirectories
            : SearchOption.TopDirectoryOnly)
        : Array.Empty<string>();

    public string[] GetFiles(string searchPattern = "*.*", bool recursive = false) => IsExists
        ? Directory.GetFiles(Value, searchPattern, recursive
            ? SearchOption.AllDirectories
            : SearchOption.TopDirectoryOnly)
        : Array.Empty<string>();

    public override bool IsExists => Directory.Exists(Value);

    public static implicit operator DirectoryReference(string path) => new DirectoryReference(path);
}
