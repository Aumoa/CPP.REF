// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CodeAnalysis/CodeDiagnostic.h"

class SyntaxTree
{
protected:
	SyntaxTree() = default;

public:
	virtual ~SyntaxTree() noexcept = default;

	virtual std::vector<CodeDiagnostic> GetDiagnostics() const = 0;
};