// Copyright 2020-2023 Aumoa.lib. All right reserved.

using AE.BuildSettings;

namespace AE.ProjectFiles.VisualStudio;

public abstract class VisualStudioProject
{
    protected VisualStudioProject()
    {
    }

    public abstract string name { get; }
    public abstract string guid { get; }
    public abstract string filter { get; }
    public abstract string project { get; }

    public abstract Task GenerateProjectFilesAsync(CancellationToken SToken = default);

    public abstract (string, string) MapConfiguration(Configuration Configuration, bool bEditor, TargetPlatform Platform);

    public abstract void ResolveDependencies(IEnumerable<VisualStudioProject> VSProjects);
}
