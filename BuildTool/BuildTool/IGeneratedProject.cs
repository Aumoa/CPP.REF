// Copyright 2020-2021 Aumoa.lib. All right reserved.

using DotNETUtilities;

namespace BuildTool;

interface IGeneratedProject
{
    public FileReference ProjectFile { get; }

    public void SaveAll();
}

