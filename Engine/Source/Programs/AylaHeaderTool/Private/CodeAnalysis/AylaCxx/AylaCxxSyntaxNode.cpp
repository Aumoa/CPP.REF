// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CodeAnalysis/AylaCxx/AylaCxxSyntaxNode.h"
#include "CodeAnalysis/AylaCxx/CxxCodeParsingContext.h"

AylaCxxSyntaxNode::AylaCxxSyntaxNode(std::defer_lock_t)
{
}

AylaCxxSyntaxNode::AylaCxxSyntaxNode(ESyntaxType InSyntaxType)
	: SyntaxType(InSyntaxType)
{
}

std::vector<CodeDiagnostic> AylaCxxSyntaxNode::GetDiagnostics() const
{
	return Diagnostics;
}

void AylaCxxSyntaxNode::SetupContext(CxxCodeParsingContext& Context)
{
	LineNumber = Context.Line;
	ColumnNumber = Context.Column;
}

#define ALLOCATE_NODE(Type) std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode(ESyntaxType::Type))

std::unique_ptr<SyntaxNode> AylaCxxSyntaxNode::ParseText(CxxCodeParsingContext& Context)
{
	Context.IgnoreWhitespaces();
	if (Context.IsEOF())
	{
		return nullptr;
	}

	auto IsStringLiteral = [&]()
	{
		switch (Context.GetChar())
		{
		case '\"':
			return true;
		case 'L':
			return Context.ScopedEquals(TEXT("L\"")) || Context.ScopedEquals(TEXT("LR\""));
		case 'u':
			return Context.ScopedEquals(TEXT("u\"")) || Context.ScopedEquals(TEXT("uR\"")) || Context.ScopedEquals(TEXT("u8\"")) || Context.ScopedEquals(TEXT("u8R\""));
		case 'U':
			return Context.ScopedEquals(TEXT("U\"")) || Context.ScopedEquals(TEXT("UR\""));
		}

		return false;
	};

	if (Context.ScopedEquals(TEXT("//")))
	{
		auto Node = ALLOCATE_NODE(Comment);
		Node->SetupContext(Context);
		Node->Code = Context.ReadLine();
		return Node;
	}
	else if (Context.ScopedEquals(TEXT("#")))
	{
		auto Node = ALLOCATE_NODE(Preprocessor);
		Node->SetupContext(Context);
		Node->Code = Context.ReadLine();
		return Node;
	}
	else if (IsStringLiteral())
	{
		auto Node = ALLOCATE_NODE(Text);
		Node->SetupContext(Context);
		Node->Code = Context.ReadStringLiteral();
		return Node;
	}
	else if (Context.OperatorChars | Linq::Contains(Context.GetChar()))
	{
		auto Node = ALLOCATE_NODE(Operator);
		Node->SetupContext(Context);
		Node->Code = Context.ExportString(1);
		return Node;
	}
	else if (Char::IsDigit(Context.GetChar()))
	{
		auto Node = ALLOCATE_NODE(Expression);
		Node->SetupContext(Context);
		Node->Code = Context.ReadExpression();
		return Node;
	}
	else
	{
		return ParseText_Identifier(Context);
	}
}

std::unique_ptr<SyntaxNode> AylaCxxSyntaxNode::ParseText_Identifier(CxxCodeParsingContext& Context)
{
	auto Node = std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode(std::defer_lock));
	Node->SetupContext(Context);
	Node->Code = Context.ReadIdentifier();
	if (Context.Keywords | Linq::Contains(Node->Code))
	{
		Node->SyntaxType = ESyntaxType::Keyword;
	}
	else
	{
		Node->SyntaxType = ESyntaxType::Identifier;
	}
	return Node;
}

#undef ALLOCATE_NODE