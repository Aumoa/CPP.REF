namespace AylaEngine;

internal static class FolderPolicy
{
    public enum PathType
    {
        Current,
        Linux,
        Windows
    }

    public static string TargetIntermediate(this GroupDescriptor descriptor, string name, ITargetInfo targetInfo, PathType pathType)
    {
        var result = Path.Combine(descriptor.IntermediateDirectory, name, targetInfo.Platform.Name, targetInfo.Config.ToString() + (targetInfo.Editor ? "-Editor" : string.Empty));
        return PathPolicy(result, pathType);
    }

    public static string ModuleIntermediate(this GroupDescriptor descriptor, string name, ITargetInfo targetInfo, BuildConfigurationProfile profile, PathType pathType)
    {
        var result = Path.Combine(descriptor.IntermediateDirectory, name, targetInfo.Platform.Name, GetConfigurationFolderName(targetInfo, profile));
        return PathPolicy(result, pathType);
    }

    public static string PlatformIntermediate(this GroupDescriptor descriptor, string name, PlatformInfo platformInfo, PathType pathType)
    {
        var result = Path.Combine(descriptor.IntermediateDirectory, name, platformInfo.Name);
        return PathPolicy(result, pathType);
    }

    public static string TargetOutput(this GroupDescriptor descriptor, ITargetInfo targetInfo, PathType pathType)
    {
        var result = Path.Combine(descriptor.BinariesDirectory, targetInfo.Platform.Name, targetInfo.Config.ToString() + (targetInfo.Editor ? "-Editor" : string.Empty));
        return PathPolicy(result, pathType);
    }

    public static string ModuleOutput(this GroupDescriptor descriptor, ITargetInfo targetInfo, BuildConfigurationProfile profile, PathType pathType)
    {
        var result = Path.Combine(descriptor.BinariesDirectory, targetInfo.Platform.Name, GetConfigurationFolderName(targetInfo, profile));
        return PathPolicy(result, pathType);
    }

    public static string TargetOutputFileName(this GroupDescriptor descriptor, Installation installation, ITargetInfo targetInfo, string projectName, ModuleType moduleType, PathType pathType)
    {
        var result = Path.Combine(TargetOutput(descriptor, targetInfo, pathType), installation.OutputFileName(projectName, moduleType));
        return PathPolicy(result, pathType);
    }

    public static string ModuleOutputFileName(this GroupDescriptor descriptor, Installation installation, ITargetInfo targetInfo, BuildConfigurationProfile profile, string projectName, ModuleType moduleType, PathType pathType)
    {
        var result = Path.Combine(ModuleOutput(descriptor, targetInfo, profile, pathType), installation.OutputFileName(projectName, moduleType));
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

    private static string GetConfigurationFolderName(ITargetInfo targetInfo, BuildConfigurationProfile profile)
    {
        return profile.Name + (targetInfo.Editor ? "-Editor" : string.Empty);
    }
}
