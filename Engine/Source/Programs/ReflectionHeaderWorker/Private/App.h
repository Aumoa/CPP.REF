// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class App
{
private:
	App() noexcept = default;

private:
	static App s_App;
	static int32 s_ExitCode;

public:
	static App& Run(CancellationToken cancellationToken);
	int32 GetExitCode() const;
};