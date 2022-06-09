// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "SourceTree.h"
#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"
#include <map>

class FileReference;
class DirectoryReference;
class SourceTree;

class BuildGraph : virtual public Object
{
	DirectoryReference _intermediateDir;
	std::map<String, std::shared_ptr<SourceTree>> _sourceTrees;

public:
	BuildGraph(const DirectoryReference& intermediateDir);

	void AddProject(const DirectoryReference& source);
};