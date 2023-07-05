// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:CoreAssert;

export import :String;

export class CORE_API CoreAssert
{
public:
	[[nodiscard]]
	static void Assert(String InExpression, String InMessage, bool bDebugbreak = true);
};