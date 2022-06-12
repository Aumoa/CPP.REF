// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "SourceTree.h"
#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"
#include <map>
#include "BuildGraph.generated.h"

class FileReference;
class DirectoryReference;
class SourceTree;

SCLASS()
class BuildGraph : virtual public Object
{
	GENERATED_BODY()

private:
	String _projectName;
	DirectoryReference _intermediateDir;
	std::map<String, std::shared_ptr<SourceTree>> _sourceTrees;

public:
	BuildGraph(const String& projectName, const DirectoryReference& intermediateDir);

	void AddProject(const DirectoryReference& source);
	Task<> Compile();
};