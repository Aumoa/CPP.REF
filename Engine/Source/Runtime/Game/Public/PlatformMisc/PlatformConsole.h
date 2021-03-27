// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GAME_API PlatformConsole : virtual public Object
{
public:
	static bool Alloc();
	static void Free();

	static void Write(TRefPtr<String> message);
	static void WriteLine(TRefPtr<String> message);
};