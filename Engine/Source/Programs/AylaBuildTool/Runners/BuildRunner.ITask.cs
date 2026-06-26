namespace AylaEngine;

internal static partial class BuildRunner
{
    internal interface ITask
    {
        GroupDescriptor Group { get; }
    }
}
