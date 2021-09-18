// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

class CORE_API SThread : virtual public SObject
{
	GENERATED_BODY(SThread)

private:
	void* _handle = nullptr;
	int64 _threadId = 0;
	std::wstring _friendlyName;

private:
	SThread();
	virtual ~SThread() override;

public:
	void SetFriendlyName(std::wstring_view friendlyName);
	std::wstring GetFriendlyName() const;
	int64 GetThreadId() const;

public:
	static SThread* GetCurrentThread();
};