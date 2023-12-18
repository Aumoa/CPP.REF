// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Source;

namespace AE.Projects;

public interface IAModule
{
    public string SourcePath { get; }

    public string ModuleName { get; }

    public bool IsInProgramsDirectory { get; }

    public ProjectDirectory ProjectDirectory { get; }
}
