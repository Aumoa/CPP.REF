// Copyright 2020-2023 Aumoa.lib. All right reserved.

import AylaHeaderTool;

InterfaceSource::InterfaceSource(String InPath)
	: Super(InPath)
{
}

Task<bool> InterfaceSource::TryParseAsync(std::stop_token InCancellationToken)
{
	String Content = co_await File::ReadAllTextAsync(GetSourcePath(), InCancellationToken);
	co_return true;
}

std::vector<String> InterfaceSource::GetErrors() const
{
	return {};
}