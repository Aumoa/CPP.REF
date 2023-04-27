// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.Diagnostics:Debug;

export import Core.System;

export class CORE_API Debug
{
public:
	static void Assert(const String& InExpression, const String& InMessage);
	static void Break();
};