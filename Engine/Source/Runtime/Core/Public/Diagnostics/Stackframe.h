// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"

class CORE_API Stackframe
{
	friend class Stacktrace;

	String _location;
	int32 _line = 0;
	String _module;
	String _description;
	int64 _address = 0;

public:
	Stackframe() noexcept;

public:
	inline String GetSourceLocation() const noexcept { return _location; }
	inline int32 GetSourceLine() const noexcept { return _line; }
	inline String GetModuleName() const noexcept { return _module; }
	inline String GetDescription() const noexcept { return _description; }
	inline int64 GetAddress() const noexcept { return _address; }
};