// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CodeAnalysis/AylaCxx/AylaCxxSyntaxTree.h"
#include "CodeAnalysis/AylaCxx/AylaCxxSyntaxNode.h"

std::vector<CodeDiagnostic> AylaCxxSyntaxTree::GetDiagnostics() const
{
	return {};
}

std::unique_ptr<SyntaxTree> AylaCxxSyntaxTree::ParseText(String Code, std::optional<String> InPath, std::stop_token InCancellationToken)
{
	auto Tree = std::unique_ptr<AylaCxxSyntaxTree>(new AylaCxxSyntaxTree());

	AylaCxxSyntaxNode::CodeParsingContext Context =
	{
		.CodePath = InPath.value_or(TEXT("<temp>")),
		.Code = Code,
		.Line = 1,
		.Column = 0,
		.Index = 0
	};

	while (Context.IsEOF() == false)
	{
		std::unique_ptr Node = AylaCxxSyntaxNode::ParseText(Context);
		if (Node == nullptr)
		{
			break;
		}
		Tree->Nodes.emplace_back(std::move(Node));
	}

	return Tree;
}