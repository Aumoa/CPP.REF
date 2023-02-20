// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Misc;
using AE.Projects;

namespace AE.ProjectFiles.VisualStudio;

public class VisualCSharpProject : IProject
{
    public string ProjectFile { get; init; }

    public string FilterPath { get; init; }

    public string Name => Path.GetFileNameWithoutExtension(ProjectFile);

    public string ProjectGuid { get; init; }

    public VisualCSharpProject(string ProjectFile, string FilterPath)
    {
        this.ProjectFile = ProjectFile;
        this.FilterPath = FilterPath;
        ProjectGuid = CRC32.GenerateGuid(ProjectFile).ToString().ToUpper();
    }
}
