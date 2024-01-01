// Copyright 2020-2023 Aumoa.lib. All right reserved.

namespace AE.IO;

public class FileReference : FileSystemReference
{
    public FileReference(string path) : base(path)
    {
    }

    public override bool IsExists => File.Exists(Value);

    public static implicit operator FileReference(string path) => new FileReference(path);
}
