// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.Diagnostics:StackFrame;

export import Core.System;

export class CORE_API StackFrame
{
	friend class StackTrace;

	String Location;
	int32 Line = 0;
	String Module;
	String Description;
	int64 AddressOf = 0;

public:
	StackFrame() noexcept {}

public:
	inline String GetSourceLocation() const noexcept { return Location; }
	inline int32 GetSourceLine() const noexcept { return Line; }
	inline String GetModuleName() const noexcept { return Module; }
	inline String GetDescription() const noexcept { return Description; }
	inline int64 GetAddress() const noexcept { return AddressOf; }
};