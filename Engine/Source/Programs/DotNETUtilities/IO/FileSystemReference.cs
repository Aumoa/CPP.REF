// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.Misc;

namespace AE.IO;

public abstract class FileSystemReference
{
    protected FileSystemReference(string path)
    {
        Value = path;
        FileName = GetFileName(AbsolutePath);
        ExtractExtensions(path, out string namePart, out string extensionsPart);
        Name = namePart;
        Extensions = extensionsPart;
    }

    public string Value { get; private set; }

    public string FileName { get; private set; }

    public string Name { get; private set; }

    public string Extensions { get; private set; }

    public string AbsolutePath => Path.GetFullPath(Value);

    public abstract bool IsExists { get; }

    private static string GetFileName(string path)
    {
        path = path.Trim(' ', '\t', Path.DirectorySeparatorChar);

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

    private static void ExtractExtensions(string fileName, out string namePart, out string extensionsPart)
    {
        int indexOf = fileName.LastIndexOf('.');
        if (indexOf == -1)
        {
            namePart = fileName;
            extensionsPart = string.Empty;
            return;
        }

        namePart = fileName[..indexOf];
        ++indexOf;
        extensionsPart = fileName.IsValidIndex(indexOf) ? fileName[indexOf..] : string.Empty;
    }

    public static implicit operator bool(FileSystemReference fr) => fr.IsExists;

    public static implicit operator string(FileSystemReference fr) => fr.Value;
}
