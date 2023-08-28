// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SourceFile
{
private:
	String SourcePath;

public:
	SourceFile(String InPath);
	virtual ~SourceFile() noexcept = default;

	virtual Task<bool> TryParseAsync(std::stop_token InCancellationToken = {}) = 0;
	virtual std::vector<String> GetErrors() const = 0;

	String GetSourcePath() const;
};