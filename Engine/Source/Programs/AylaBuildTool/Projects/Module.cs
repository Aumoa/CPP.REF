// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Source;

namespace AE.Projects;

public abstract class Module
{
    public abstract string SourcePath { get; }

    public abstract string Name { get; }

    public abstract ProjectDirectory ProjectDirectory { get; }

    public bool isProgram => SourcePath.StartsWith(ProjectDirectory.Source.Programs);
}
