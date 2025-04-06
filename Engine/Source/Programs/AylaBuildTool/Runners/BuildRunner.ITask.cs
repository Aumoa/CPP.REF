namespace AylaEngine;

internal static partial class BuildRunner
{
    private interface ITask
    {
        GroupDescriptor Group { get; }
    }
}
