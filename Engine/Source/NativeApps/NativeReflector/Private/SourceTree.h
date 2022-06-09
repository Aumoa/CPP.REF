// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "IO/DirectoryReference.h"
#include "Threading/Tasks/Task.h"
#include <memory>
#include <map>
#include <stop_token>

class SourceTree : virtual public Object
{
	DirectoryReference _projectDir;

	std::vector<std::shared_ptr<SourceTree>> _dependencies;
	std::map<String, void*> _sourceTree;

public:
	SourceTree(const DirectoryReference& projectDir);

	void AddDependencySourceTree(std::shared_ptr<SourceTree> sourceTree);
	Task<> CompileAsync(std::stop_token sToken = {});

private:
	Task<> CompileWorker(FileReference fr, std::stop_token sToken);

private:
	void Interpreter_SCLASS(const String& sourceCode, size_t& indexOf, size_t& line);
};