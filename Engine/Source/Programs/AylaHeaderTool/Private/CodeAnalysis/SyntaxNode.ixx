// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module AylaHeaderTool:SyntaxNode;

import :CodeDiagnostic;

export class SyntaxNode
{
protected:
	SyntaxNode() = default;

public:
	virtual ~SyntaxNode() noexcept = default;

	virtual std::vector<CodeDiagnostic> GetDiagnostics() const = 0;
};