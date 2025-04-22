namespace AylaEngine;

internal static class FolderPolicy
{
    public enum PathType
    {
        Current,
        Linux,
        Windows
    }

    public static string Intermediate(this GroupDescriptor descriptor, string name, ITargetInfo targetInfo, PathType pathType)
    {
        var result = Path.Combine(descriptor.IntermediateDirectory, name, targetInfo.Platform.Name, targetInfo.Config.ToString() + (targetInfo.Editor ? "-Editor" : string.Empty));
        return PathPolicy(result, pathType);
    }

    public static string Output(this GroupDescriptor descriptor, ITargetInfo targetInfo, PathType pathType)
    {
        var result = Path.Combine(descriptor.BinariesDirectory, targetInfo.Platform.Name, targetInfo.Config.ToString() + (targetInfo.Editor ? "-Editor" : string.Empty));
        return PathPolicy(result, pathType);
    }

    public static string OutputFileName(string projectName, ModuleType moduleType)
    {
        return projectName + moduleType switch
        {
            ModuleType.Library => ".dll",
            ModuleType.Game => ".dll",
            ModuleType.Application => ".exe",
            _ => string.Empty
        };
    }

    public static string OutputFileName(this GroupDescriptor descriptor, ITargetInfo targetInfo, string projectName, ModuleType moduleType, PathType pathType)
    {
        var result = Path.Combine(Output(descriptor, targetInfo, pathType), OutputFileName(projectName, moduleType));
        return PathPolicy(result, pathType);
    }

    public static string Unused(this GroupDescriptor descriptor, PathType pathType)
    {
        var result = Path.Combine(descriptor.IntermediateDirectory, "Unused");
        return PathPolicy(result, pathType);
    }

    private static string PathPolicy(string value, PathType pathType)
    {
        switch (pathType)
        {
            case PathType.Linux:
                return value.Replace(Path.DirectorySeparatorChar, '/');
            case PathType.Windows:
                return value.Replace(Path.DirectorySeparatorChar, '\\');
            default:
                return value;
        }
    }
}
