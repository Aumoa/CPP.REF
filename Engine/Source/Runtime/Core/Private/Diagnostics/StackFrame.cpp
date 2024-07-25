// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:StackFrame;

export import :IntegralTypes;
export import :String;

export struct CORE_API StackFrame
{
	String Location;
	int32 Line = 0;
	String Module;
	String Description;
	int64 AddressOf = 0;
};