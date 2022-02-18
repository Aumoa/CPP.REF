// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Reflection/Enum.h"
#include <chrono>

class CORE_API SEventHandle : implements SObject
{
	GENERATED_BODY(SEventHandle)

public:
	SENUM(EStatus, int32,
		Succeeded,,
		Timeout,,
		Failed,
	);

private:
	void* _handle = nullptr;

public:
	SEventHandle();
	~SEventHandle() override;

	EStatus Wait(std::chrono::milliseconds timeout = 0xFFFFFFFFms);
	void Set();
	void Reset();
	void* GetHandle() const { return _handle; }
};