// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NSCWApp
{
public:
	NSCWApp();

	Task<> RunAsync(std::stop_token InCancellationToken);
	void PrintUsage(TextWriter& Writer);
};