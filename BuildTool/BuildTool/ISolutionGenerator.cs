// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace BuildTool;

interface ISolutionGenerator
{
    IGeneratedSolution Generate(Solution CompiledSolution);
}