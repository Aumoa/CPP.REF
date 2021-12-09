// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "NonCopyable.h"
#include <string>

class CORE_API Thread : public NonCopyable
{
	void* _handle = nullptr;
	int64 _threadId = 0;
	std::wstring _friendlyName;

private:
	Thread();

public:
	void SetFriendlyName(std::wstring_view friendlyName);
	std::wstring GetFriendlyName() const;
	int64 GetThreadId() const;

public:
	static Thread* GetCurrentThread();
};