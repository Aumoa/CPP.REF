// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Sources/SourceFile.h"

class SyntaxTree;

class HeaderSource : public SourceFile
{
protected:
	using Super = SourceFile;

private:
	std::unique_ptr<SyntaxTree> Syntaxes;

public:
	HeaderSource(String InPath);
	virtual ~HeaderSource() noexcept override;

	virtual Task<bool> TryParseAsync(std::stop_token InCancellationToken) override;
	virtual std::vector<String> GetErrors() const override;
	virtual Task<bool> CompileAsync(std::stop_token InCancellationToken) override;
	virtual Task<> GenerateAsync(std::stop_token InCancellationToken = {}) override;
};