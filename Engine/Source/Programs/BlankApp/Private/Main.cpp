// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"

int main()
{
	PlatformProcess::SetupStacktraceSignals();
	Console::WriteLine(TEXT("BlankApp"));
	return 0;
}