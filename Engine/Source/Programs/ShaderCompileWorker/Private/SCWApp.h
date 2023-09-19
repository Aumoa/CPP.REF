// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "SCWApp.generated.h"

ACLASS()
class ASCWApp : public AObject
{
	GENERATED_BODY()

public:
	ASCWApp();

	Task<int32> RunAsync(std::stop_token InCancellationToken);
	void PrintUsage(TextWriter& Writer);
};