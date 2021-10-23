// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class ObjectCollection
{
	friend class SObject;

	static std::vector<std::shared_ptr<SObject>> MarkRemoved;

	static void AddMarkedObject(SObject* InObject);
	static void RemoveMarkedObject(SObject* InObject);

public:
	static void Collect();
};