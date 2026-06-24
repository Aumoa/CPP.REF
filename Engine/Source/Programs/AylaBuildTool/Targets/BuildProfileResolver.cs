namespace AylaEngine;

internal static class BuildProfileResolver
{
    public static BuildConfigurationProfile Resolve(ModuleProject project, ITargetInfo targetInfo)
    {
        return Resolve(project.Group, targetInfo);
    }

    public static BuildConfigurationProfile Resolve(GroupDescriptor group, ITargetInfo targetInfo)
    {
        return group.Kind switch
        {
            SourceGroupKind.Engine => targetInfo.Config.GetEngineProfile(),
            SourceGroupKind.Project => targetInfo.Config.GetGameProfile(),
            _ => throw new ArgumentOutOfRangeException(nameof(group), group.Kind, null)
        };
    }
}
