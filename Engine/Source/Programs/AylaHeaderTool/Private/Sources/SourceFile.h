// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SyntaxNode;

class SourceFile
{
	SourceFile(const SourceFile&) = delete;
	SourceFile(SourceFile&&) = delete;

private:
	String SourcePath;

public:
	SourceFile(String InPath);
	virtual ~SourceFile() noexcept = default;

	virtual Task<bool> TryParseAsync(std::stop_token InCancellationToken = {}) = 0;
	virtual std::vector<String> GetErrors() const = 0;
	virtual Task<bool> CompileAsync(std::stop_token InCancellationToken = {}) = 0;
	virtual Task<> GenerateAsync(String IncludePath, String CSharpPath, std::stop_token InCancellationToken = {}) = 0;

	String GetSourcePath() const;
};