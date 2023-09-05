// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Sources/HeaderSource.h"
#include "CodeAnalysis/SyntaxTree.h"
#include "CodeAnalysis/AylaCxx/AylaCxxSyntaxTree.h"

HeaderSource::HeaderSource(String InPath)
	: Super(InPath)
{
}

HeaderSource::~HeaderSource() noexcept
{
}

Task<bool> HeaderSource::TryParseAsync(std::stop_token InCancellationToken)
{
	String Content = co_await File::ReadAllTextAsync(GetSourcePath(), InCancellationToken);
	Syntaxes = AylaCxxSyntaxTree::ParseText(Content, GetSourcePath(), InCancellationToken);
	std::vector<CodeDiagnostic> Diagnostics = Syntaxes->GetDiagnostics();
	if (Diagnostics.size() > 0)
	{
		co_return false;
	}
	co_return true;
}

std::vector<String> HeaderSource::GetErrors() const
{
	return Syntaxes->GetDiagnostics() | Linq::Select([](const CodeDiagnostic& Diag)
	{
		return Diag.ToString();
	}) | Linq::ToVector();
}

Task<bool> HeaderSource::CompileAsync(std::stop_token InCancellationToken)
{
	co_return false;
}

Task<> HeaderSource::GenerateAsync(std::stop_token InCancellationToken)
{
	return Task<>::CompletedTask();
}