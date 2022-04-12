// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Console
{
	class CONSOLE_API SConsoleModuleSubsystem : virtual public SObject
	{
		GENERATED_BODY(SConsoleModuleSubsystem);

	public:
		SConsoleModuleSubsystem();

		virtual void Init();
		virtual void Deinit();
	};
}