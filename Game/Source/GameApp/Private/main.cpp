// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "GameApp.h"

int main(int argc, char** argv)
{
	return ConsoleApplication::GuardedMain<GameApp>(argc, argv);
}