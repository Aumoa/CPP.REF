namespace AylaEngine;

internal readonly struct SourceCodeDescriptor(string FilePath, string RelativePath, SourceCodeType Type)
{
    public readonly string FilePath = FilePath;
    public readonly string RelativePath = RelativePath;
    public readonly string Directory = Path.GetDirectoryName(FilePath) ?? throw new InvalidOperationException();
    public readonly string RelativeDirectory = Path.GetDirectoryName(RelativePath) ?? throw new InvalidOperationException();
    public readonly SourceCodeType Type = Type;

    public static bool TryGet(string filePath, string sourceCodeRoot, out SourceCodeDescriptor outValue)
    {
        var extensions = Path.GetExtension(filePath).ToLower();
        switch (extensions)
        {
            case ".cpp":
                outValue = new SourceCodeDescriptor(filePath, Path.GetRelativePath(sourceCodeRoot, filePath), SourceCodeType.SourceCode);
                return true;
            case ".h":
                outValue = new SourceCodeDescriptor(filePath, Path.GetRelativePath(sourceCodeRoot, filePath), SourceCodeType.Header);
                return true;
            case ".cs":
                outValue = new SourceCodeDescriptor(filePath, Path.GetRelativePath(sourceCodeRoot, filePath), SourceCodeType.Declaration);
                return true;
        }

        outValue = default;
        return false;
    }
}
