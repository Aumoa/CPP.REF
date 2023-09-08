// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IEngineLoop.h"

struct COREED_API NCoreEd : public StaticClass
{
	static void EditorInit(IEngineLoop& Loop);
};