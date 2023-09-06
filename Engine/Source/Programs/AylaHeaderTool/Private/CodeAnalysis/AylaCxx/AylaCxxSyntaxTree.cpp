// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CodeAnalysis/AylaCxx/AylaCxxSyntaxTree.h"
#include "CodeAnalysis/AylaCxx/AylaCxxSyntaxNode.h"
#include "CodeAnalysis/AylaCxx/CxxCodeParsingContext.h"

AylaCxxSyntaxTree::AylaCxxSyntaxTree()
{
}

AylaCxxSyntaxTree::~AylaCxxSyntaxTree() noexcept
{
}

std::vector<CodeDiagnostic> AylaCxxSyntaxTree::GetDiagnostics() const
{
	std::vector<CodeDiagnostic> Diags;
	for (auto& Node : Nodes)
	{
		auto SubDiags = Node->GetDiagnostics();
		Diags.insert(Diags.end(), SubDiags.begin(), SubDiags.end());
	}
	return Diags;
}

std::span<const std::unique_ptr<SyntaxNode>> AylaCxxSyntaxTree::GetNodes() const
{
	return Nodes;
}

std::unique_ptr<SyntaxTree> AylaCxxSyntaxTree::ParseText(String Code, std::optional<String> InPath, std::stop_token InCancellationToken)
{
	auto Tree = std::unique_ptr<AylaCxxSyntaxTree>(new AylaCxxSyntaxTree());

	CxxCodeParsingContext Context;
	Context.Init(InPath.value_or(TEXT("<temp>")), Code);

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