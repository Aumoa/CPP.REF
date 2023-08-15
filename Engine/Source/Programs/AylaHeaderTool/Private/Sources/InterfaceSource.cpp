// Copyright 2020-2023 Aumoa.lib. All right reserved.

import AylaHeaderTool;

InterfaceSource::InterfaceSource(String InPath)
	: Super(InPath)
{
}

Task<bool> InterfaceSource::TryParseAsync(std::stop_token InCancellationToken)
{
	String Content = co_await File::ReadAllTextAsync(GetSourcePath(), InCancellationToken);
	std::unique_ptr<SyntaxTree> SyntaxTree = AylaCxxSyntaxTree::ParseText(Content, GetSourcePath(), InCancellationToken);
	std::vector<CodeDiagnostic> Diagnostics = SyntaxTree->GetDiagnostics();
	if (Diagnostics.size() > 0)
	{
		co_return false;
	}
	co_return true;
}

std::vector<String> InterfaceSource::GetErrors() const
{
	return {};
}