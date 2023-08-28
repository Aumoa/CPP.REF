// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Sources/SourceFile.h"

class InterfaceSource : public SourceFile
{
protected:
	using Super = SourceFile;

public:
	InterfaceSource(String InPath);

	virtual Task<bool> TryParseAsync(std::stop_token InCancellationToken) override;
	virtual std::vector<String> GetErrors() const override;
};