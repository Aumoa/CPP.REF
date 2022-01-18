// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace DotNETUtilities;

using System;
using System.IO;

public class FileReference : FileSystemReference
{
    public FileReference(string InPath) : base(InPath)
    {
        Extension = Path.GetExtension(FullPath);

        if (Extension is not null)
        {
            FileName = $"{Name}{Extension}";
        }
        else
        {
            FileName = Name;
        }
    }

    public bool CompareExtension(string InExtension)
    {
        if (InExtension[0] is not '.')
        {
            InExtension = $".{InExtension}";
        }

        return Extension?.Equals(InExtension, StringComparison.OrdinalIgnoreCase) == true;
    }

    public bool IsExist
    {
        get => File.Exists(FullPath);
    }

    public string FileName { get; }

    public string Extension { get; }

    public string ReadAllText()
    {
        return File.ReadAllText(FullPath);
    }
}