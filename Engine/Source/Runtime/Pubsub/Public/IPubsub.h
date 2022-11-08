// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPubsub.gen.h"

SINTERFACE()
interface PUBSUB_API IPubsub : implements Object
{
	GENERATED_BODY()

public:
	virtual void Publish(const String& topic, const String& payload) = 0;
	virtual void Subscribe(const String& topic, std::function<void(String)> func) = 0;
};