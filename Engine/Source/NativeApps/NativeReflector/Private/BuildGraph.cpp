// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "BuildGraph.h"

BuildGraph::BuildGraph(const DirectoryReference& intermediateDir)
	: _intermediateDir(intermediateDir)
{
}

void BuildGraph::AddProject(const DirectoryReference& source)
{
	String projectName = source.GetParent()->GetName();
	auto it = _sourceTrees.emplace(projectName, std::make_shared<SourceTree>(source));
	it.first->second->CompileAsync().GetResult();
}