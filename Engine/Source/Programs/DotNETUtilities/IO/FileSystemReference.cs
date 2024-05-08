// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Misc;

namespace AE.IO;

public abstract class FileSystemReference : IEquatable<FileSystemReference>
{
    protected FileSystemReference(string path)
    {
        Value = path;
        FileName = GetFileName(AbsolutePath);
        HasExtensions = ExtractExtensions(FileName, out string namePart, out string extensionsPart);
        Name = namePart;
        Extensions = extensionsPart;
    }

    public string Value { get; private set; }

    public string FileName { get; private set; }

    public string Name { get; private set; }

    public string Extensions { get; private set; }

    public bool HasExtensions { get; private set; }

    public string AbsolutePath => Path.GetFullPath(Value);

    public abstract bool IsExists { get; }

    public bool IsPathFullQualified => Path.IsPathFullyQualified(Value);

    public abstract FileSystemReference Absolute { get; }

    public abstract FileSystemReference ToCurrentDirectoryBased();

    public bool Includes(FileSystemReference fr)
    {
        if (fr.Value.StartsWith(Value, StringComparison.OrdinalIgnoreCase))
        {
            return true;
        }

        if (fr.AbsolutePath.StartsWith(AbsolutePath, StringComparison.OrdinalIgnoreCase))
        {
            return true;
        }

        return false;
    }

    public string GetRelativePath(FileSystemReference fr)
    {
        return Path.GetRelativePath(fr.Value, Value);
    }

    public bool Equals(FileSystemReference? rhs)
    {
        if (ReferenceEquals(this, rhs))
        {
            return true;
        }

        if (ReferenceEquals(rhs, null))
        {
            return false;
        }

        if (Value.Equals(rhs.Value, StringComparison.OrdinalIgnoreCase))
        {
            return true;
        }

        if (AbsolutePath.Equals(rhs.AbsolutePath, StringComparison.OrdinalIgnoreCase))
        {
            return true;
        }

        return false;
    }

    public override bool Equals(object? obj)
    {
        if (ReferenceEquals(this, obj))
        {
            return true;
        }

        if (ReferenceEquals(obj, null))
        {
            return false;
        }

        if (obj is FileSystemReference fsr)
        {
            return Equals(fsr);
        }

        return false;
    }

    public override int GetHashCode()
    {
        return Value.ToLower().GetHashCode();
    }

    public override string ToString()
    {
        return Value;
    }

    protected static string TrimPath(string path) => path.Trim(' ', '\t', Path.DirectorySeparatorChar);

    private static string GetFileName(string path)
    {
        path = TrimPath(path);

        int indexOf = path.LastIndexOf(Path.DirectorySeparatorChar);
        if (indexOf == -1)
        {
            return path;
        }

        int nextPos = indexOf + 1;
        if (path.IsValidIndex(nextPos) == false)
        {
            return string.Empty;
        }

        return path[nextPos..];
    }

    private static bool ExtractExtensions(string fileName, out string namePart, out string extensionsPart)
    {
        int indexOf = fileName.LastIndexOf('.');
        if (indexOf == -1)
        {
            namePart = fileName;
            extensionsPart = string.Empty;
            return false;
        }

        namePart = fileName[..indexOf];
        ++indexOf;
        extensionsPart = fileName.IsValidIndex(indexOf) ? fileName[indexOf..] : string.Empty;
        return true;
    }

    public static implicit operator bool(FileSystemReference fr) => fr.IsExists;

    public static implicit operator string(FileSystemReference fr) => fr.Value;

    public static bool operator ==(FileSystemReference? lhs, FileSystemReference? rhs)
    {
        if (ReferenceEquals(lhs, rhs))
        {
            return true;
        }

        if (ReferenceEquals(lhs, null) || ReferenceEquals(rhs, null))
        {
            return false;
        }

        return lhs.Equals(rhs);
    }

    public static bool operator !=(FileSystemReference? lhs, FileSystemReference? rhs)
    {
        if (ReferenceEquals(lhs, rhs))
        {
            return false;
        }

        if (ReferenceEquals(lhs, null) || ReferenceEquals(rhs, null))
        {
            return true;
        }

        return lhs.Equals(rhs) == false;
    }
}
