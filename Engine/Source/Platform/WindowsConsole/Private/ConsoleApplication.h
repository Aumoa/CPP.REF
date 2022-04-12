// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Windows
{
	class SConsoleApplication : extends(SObject)
	{
		GENERATED_BODY(SConsoleApplication)

	public:
		static int32 GuardedMain(std::span<const std::wstring> Argv);
	};
}