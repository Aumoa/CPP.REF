// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CodeAnalysis/CodeDiagnostic.h"

class SyntaxNode;

class SyntaxTree
{
	SyntaxTree(const SyntaxTree&) = delete;
	SyntaxTree(SyntaxTree&&) = delete;

protected:
	SyntaxTree() = default;

public:
	virtual ~SyntaxTree() noexcept = default;

	virtual std::vector<CodeDiagnostic> GetDiagnostics() const = 0;
	virtual std::span<const std::unique_ptr<SyntaxNode>> GetNodes() const = 0;
};