// Copyright 2020-2023 Aumoa.lib. All right reserved.

import AylaHeaderTool;

AylaCxxSyntaxNode::AylaCxxSyntaxNode()
{
}

std::vector<CodeDiagnostic> AylaCxxSyntaxNode::GetDiagnostics() const
{
	return {};
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
	else
	{
		Context.Index = Context.Code.length();
		auto Node = std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode());
		Node->Diagnostics.emplace_back(CodeDiagnostic(Context.CodePath, Context.Line, Context.Column, ErrorCode_UnknownSyntaxExpression, Message_UnknownSyntaxExpression));
		return Node;
	}
}

std::unique_ptr<SyntaxNode> AylaCxxSyntaxNode::ParseText_Comment(CodeParsingContext& Context)
{
	size_t StartIndex = Context.Index;
	String Comment;

	while (Context.IsEOF() == false)
	{
		const char_t C = Context.Code[Context.Index];
		Context.Increment();
		if (C == '\n')
		{
			Comment = Context.Code.Substring(StartIndex, Context.Index - StartIndex).TrimEnd();
			Context.Increment();
			break;
		}
	}

	if (Comment.IsEmpty())
	{
		Comment = Context.Code.Substring(StartIndex, Context.Index - StartIndex);
	}

	auto Node = std::unique_ptr<AylaCxxSyntaxNode>(new AylaCxxSyntaxNode());
	Node->Code = Comment;
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