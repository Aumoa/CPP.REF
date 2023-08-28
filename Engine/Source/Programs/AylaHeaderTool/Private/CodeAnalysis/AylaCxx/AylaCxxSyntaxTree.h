// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CodeAnalysis/SyntaxTree.h"
#include "CodeAnalysis/SyntaxNode.h"

class AylaCxxSyntaxTree : public SyntaxTree
{
private:
	std::vector<std::unique_ptr<SyntaxNode>> Nodes;

private:
	AylaCxxSyntaxTree() = default;

public:
	virtual std::vector<CodeDiagnostic> GetDiagnostics() const override;

public:
	static std::unique_ptr<SyntaxTree> ParseText(String Code, std::optional<String> InPath, std::stop_token InCancellationToken = {});
};