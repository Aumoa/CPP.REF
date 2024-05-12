// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public abstract class VisualStudioProject
{
    protected VisualStudioProject()
    {
    }

    public abstract string Name { get; }

    public abstract string GUID { get; }

    public abstract string Filter { get; }

    public string ProjectPath => Filter + '/' + Name;

    public abstract FileReference Project { get; }

    public abstract ProjectDirectory TargetDirectory { get; }

    public abstract DirectoryReference SourceDirectory { get; }

    public abstract Task GenerateProjectFilesAsync(List<VisualStudioProject> projects, CancellationToken cancellationToken = default);

    public abstract (string, string) MapConfiguration(Configuration configuration, bool isEditor, TargetPlatform platform);
}
