// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module AylaHeaderTool:InterfaceSource;

import Core;
import :SourceFile;

export class InterfaceSource : public SourceFile
{
protected:
	using Super = SourceFile;

public:
	InterfaceSource(String InPath);

	virtual Task<bool> TryParseAsync(std::stop_token InCancellationToken) override;
	virtual std::vector<String> GetErrors() const override;
};