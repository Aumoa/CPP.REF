// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module AylaHeaderTool:SyntaxTree;

import :CodeDiagnostic;

export class SyntaxTree
{
protected:
	SyntaxTree() = default;

public:
	virtual ~SyntaxTree() noexcept = default;

	virtual std::vector<CodeDiagnostic> GetDiagnostics() const = 0;
};