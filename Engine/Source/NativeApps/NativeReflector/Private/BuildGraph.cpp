// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "BuildGraph.h"

BuildGraph::BuildGraph(const String& projectName, const DirectoryReference& intermediateDir)
	: _projectName(projectName)
	, _intermediateDir(intermediateDir)
{
}

void BuildGraph::AddProject(const DirectoryReference& source)
{
	String projectName = source.GetParent()->GetName();
	auto it = _sourceTrees.emplace(projectName, std::make_shared<SourceTree>(projectName, source, _intermediateDir));
}

Task<> BuildGraph::Compile()
{
	std::vector<Task<>> tasks;

	for (auto& [projectName, sourceTree] : _sourceTrees)
	{
		tasks.emplace_back(sourceTree->CompileAsync());
	}

	co_await Task<>::WhenAll(tasks);
}