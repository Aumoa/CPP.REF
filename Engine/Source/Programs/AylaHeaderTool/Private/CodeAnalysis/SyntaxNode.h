// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CodeAnalysis/CodeDiagnostic.h"

class SyntaxNode
{
protected:
	SyntaxNode() = default;

public:
	virtual ~SyntaxNode() noexcept = default;

	virtual std::vector<CodeDiagnostic> GetDiagnostics() const = 0;
};