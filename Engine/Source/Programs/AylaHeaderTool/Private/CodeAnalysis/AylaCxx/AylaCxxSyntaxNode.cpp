// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CodeAnalysis/AylaCxx/AylaCxxSyntaxNode.h"

AylaCxxSyntaxNode::AylaCxxSyntaxNode(ESyntaxType InSyntaxType)
	: SyntaxType(InSyntaxType)
{
}

AylaCxxSyntaxNode::ESyntaxType AylaCxxSyntaxNode::GetSyntaxType() const
{
	return SyntaxType;
}

std::vector<CodeDiagnostic> AylaCxxSyntaxNode::GetDiagnostics() const
{
	return Diagnostics;
}

std::unique_ptr<SyntaxNode> AylaCxxSyntaxNode::ParseText(CodeParsingContext& Context)
{
	IgnoreWhitespaces(Context);
	if (Context.IsEOF())
	{
		return nullptr;
	}

	if (Context.Equals(TEXT("//")))
	{
		return ParseText_Comment(Context);
	}
	else if (Context.Equals(TEXT("#")))
	{
		return ParseText_Preprocessor(Context);
	}
	else if (Context.OperatorChars | Linq::Contains(Context.GetChar()))
	{
		return ParseText_Operator(Context);
	}
	else
	{
		return ParseText_Identifier(Context);
	}
}

std::unique_ptr<SyntaxNode> AylaCxxSyntaxNode::ParseText_Comment(CodeParsingContext& Context)
{
	auto Node = std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode(ESyntaxType::Comment));
	Node->Code = Context.ReadLine();
	return Node;
}

std::unique_ptr<SyntaxNode> AylaCxxSyntaxNode::ParseText_Preprocessor(CodeParsingContext& Context)
{
	auto Node = std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode(ESyntaxType::Preprocessor));
	Node->Code = Context.ReadLine();
	return Node;
}

std::unique_ptr<SyntaxNode> AylaCxxSyntaxNode::ParseText_Identifier(CodeParsingContext& Context)
{
	String Identifier = Context.ReadIdentifier();
	std::unique_ptr<AylaCxxSyntaxNode> Node;
	if (Context.Keywords | Linq::Contains(Identifier))
	{
		Node = std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode(ESyntaxType::Keyword));
	}
	else
	{
		Node = std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode(ESyntaxType::Identifier));
	}
	Node->Code = Identifier;
	return Node;
}

std::unique_ptr<SyntaxNode> AylaCxxSyntaxNode::ParseText_Operator(CodeParsingContext& Context)
{
	auto Node = std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode(ESyntaxType::Operator));
	Node->Code = Context.ConsumeString(1);
	return Node;
}

void AylaCxxSyntaxNode::IgnoreWhitespaces(CodeParsingContext& Context)
{
	while (Context.IsEOF() == false)
	{
		char_t C = Context.Code[Context.Index];
		if (Char::IsWhiteSpace(C) == false)
		{
			break;
		}

		Context.Increment();
	}
}