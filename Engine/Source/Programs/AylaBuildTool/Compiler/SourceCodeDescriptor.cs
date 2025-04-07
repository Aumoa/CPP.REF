namespace AylaEngine;

internal readonly struct SourceCodeDescriptor(GroupDescriptor Group, string ModuleName, string FilePath, string RelativePath, SourceCodeType Type)
{
    public readonly GroupDescriptor Group = Group;
    public readonly string ModuleName = ModuleName;
    public readonly string FilePath = FilePath;
    public readonly string RelativePath = RelativePath;
    public readonly string Directory = Path.GetDirectoryName(FilePath) ?? throw new InvalidOperationException();
    public readonly string RelativeDirectory = Path.GetDirectoryName(RelativePath) ?? throw new InvalidOperationException();
    public readonly SourceCodeType Type = Type;

    public static bool TryGet(GroupDescriptor group, string moduleName, string filePath, string sourceCodeRoot, out SourceCodeDescriptor outValue)
    {
        var extensions = Path.GetExtension(filePath).ToLower();
        SourceCodeType type;
        switch (extensions)
        {
            case ".cpp":
                type = SourceCodeType.SourceCode;
                break;
            case ".h":
                type = SourceCodeType.Header;
                break;
            case ".cs":
                type = SourceCodeType.Declaration;
                break;
            case ".natvis":
                type = SourceCodeType.NativeVisualizer;
                break;
            default:
                outValue = default;
                return false;
        }

        outValue = new SourceCodeDescriptor(group, moduleName, filePath, Path.GetRelativePath(sourceCodeRoot, filePath), type);
        return true;
    }

    public static SourceCodeDescriptor Get(GroupDescriptor group, string moduleName, string filePath, string sourceCodeRoot)
    {
        if (TryGet(group, moduleName, filePath, sourceCodeRoot, out var result) == false)
        {
            throw TerminateException.Internal();
        }

        return result;
    }
}
