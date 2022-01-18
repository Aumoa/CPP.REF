// Copyright 2020-2021 Aumoa.lib. All right reserved.

using DotNETUtilities;

namespace BuildTool;

interface IGeneratedSolution
{
    void SaveAll();

    FileReference SolutionFile { get; }
}