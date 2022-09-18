// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AActor;

struct SerializedLevelSpawn
{
	SubclassOf<AActor> ActorClass;
	Transform RootComponentTransform;
	std::map<String, RefPtr<Object>> SerializedFields;
};