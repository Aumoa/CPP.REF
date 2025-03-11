// Copyright 2020-2025 Aumoa.lib. All right reserved.

namespace AylaEngine;

public class FileReference : FileSystemReference
{
    public FileReference(string path) : base(path)
    {
    }

    public override bool IsExists => File.Exists(Value);

    public override FileReference ToCurrentDirectoryBased()
    {
        return new FileReference(Path.Combine(Environment.CurrentDirectory, Value));
    }

    public void Delete()
    {
        File.Delete(Value);
    }

    public FileReference WithExtensions(string? extensions)
    {
        if (string.IsNullOrEmpty(extensions))
        {
            extensions = string.Empty;
        }
        else if (extensions[0] != '.')
        {
            extensions = '.' + extensions;
        }

        return Path.Combine(GetDirectory() ?? string.Empty, $"{Name}{extensions}");
    }

    public DirectoryReference GetDirectory()
    {
        var directoryName = Path.GetDirectoryName(Value);
        return directoryName ?? throw new InvalidOperationException($"Directory {directoryName} has no parent.");
    }

    public Task<string> ReadAllTextAsync(CancellationToken cancellationToken = default)
    {
        return File.ReadAllTextAsync(Value, cancellationToken);
    }

    public Task<byte[]> ReadAllBytesAsync(CancellationToken cancellationToken = default)
    {
        return File.ReadAllBytesAsync(Value, cancellationToken);
    }

    public Task WriteAllTextAsync(string contents, CancellationToken cancellationToken = default)
    {
        return File.WriteAllTextAsync(Value, contents, cancellationToken);
    }

    public override FileReference Absolute => new(AbsolutePath);

    public static implicit operator FileReference(string path) => new FileReference(path);
}
