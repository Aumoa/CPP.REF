// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "CompiledSCLASS.h"
#include "IO/DirectoryReference.h"
#include "Threading/Tasks/Task.h"
#include <memory>
#include <map>
#include <stop_token>

class SourceTree : virtual public Object
{
	String _projectName;
	DirectoryReference _projectDir;
	DirectoryReference _intermediateDir;

	std::vector<std::shared_ptr<SourceTree>> _dependencies;
	std::map<String, void*> _sourceTree;

public:
	SourceTree(const String& projectName, const DirectoryReference& projectDir, const DirectoryReference& intermediateDir);

	void AddDependencySourceTree(std::shared_ptr<SourceTree> sourceTree);
	Task<> CompileAsync(std::stop_token sToken = {});

private:
	void CompileWorker(const FileReference& fr, std::stop_token sToken);

private:
	std::unique_ptr<CompiledSCLASS> Interpreter_SCLASS(const String& sourceCode, size_t& indexOf, size_t& line);
};