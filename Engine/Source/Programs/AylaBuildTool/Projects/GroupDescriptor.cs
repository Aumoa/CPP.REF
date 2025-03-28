namespace AylaEngine;

internal record GroupDescriptor
{
    public string RootDirectory { get; init; } = string.Empty;

    public string Name { get; init; } = string.Empty;

    public bool IsEngine { get; init; }

    public string SourceDirectory { get; init; } = string.Empty;

    public string IntermediateDirectory { get; init; } = string.Empty;

    public string BinariesDirectory { get; init; } = string.Empty;

    public string ContentDirectory { get; init; } = string.Empty;

    public static GroupDescriptor FromRoot(string rootPath, bool isEngine)
    {
        return new GroupDescriptor
        {
            RootDirectory = rootPath,
            Name = Path.GetFileName(rootPath),
            IsEngine = isEngine,
            SourceDirectory = Path.Combine(rootPath, "Source"),
            IntermediateDirectory = Path.Combine(rootPath, "Intermediate"),
            BinariesDirectory = Path.Combine(rootPath, "Binaries"),
            ContentDirectory = Path.Combine(rootPath, "Content")
        };
    }
}
