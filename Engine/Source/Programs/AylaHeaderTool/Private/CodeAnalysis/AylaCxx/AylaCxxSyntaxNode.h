// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CodeAnalysis/SyntaxNode.h"

class AylaCxxSyntaxTree;
struct CxxCodeParsingContext;

class AylaCxxSyntaxNode : public SyntaxNode
{
	friend AylaCxxSyntaxTree;

public:
	enum class ESyntaxType
	{
		None,
		Comment,
		Preprocessor,
		Identifier,
		Operator,
		Keyword,
		Expression,
		Text
	};

private:
	std::vector<CodeDiagnostic> Diagnostics;
	String Code;
	int32 LineNumber = 0;
	int32 ColumnNumber = 0;
	ESyntaxType SyntaxType = ESyntaxType::None;

public:
	static constexpr int32 ErrorCode_UnknownSyntaxExpression = 1;
	static constexpr String Message_UnknownSyntaxExpression = TEXT("Unknown Syntax Expression");

private:
	AylaCxxSyntaxNode(std::defer_lock_t);
	AylaCxxSyntaxNode(ESyntaxType InSyntaxType);

public:
	inline ESyntaxType GetSyntaxType() const { return SyntaxType; }
	inline String GetCode() const { return Code; }
	inline int32 GetLineNumber() const { return LineNumber; }
	inline int32 GetColumnNumber() const { return ColumnNumber; }

	virtual std::vector<CodeDiagnostic> GetDiagnostics() const override;

private:
	void SetupContext(CxxCodeParsingContext& Context);

public:
	static std::unique_ptr<SyntaxNode> ParseText(CxxCodeParsingContext& Context);

private:
	static std::unique_ptr<SyntaxNode> ParseText_Identifier(CxxCodeParsingContext& Context);
};