// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.BuildSettings;

namespace AE.ProjectFiles.VisualStudio;

public interface IVisualStudioProject
{
    string TargetName { get; }

    string ProjectGuid { get; }

    string FilterPath { get; }

    string ProjectFile { get; }

    (string, string) MapConfiguration(Configuration Configuration, bool bEditor, TargetPlatform Platform);

    Task GenerateProjectFilesAsync(CancellationToken SToken = default);
}
