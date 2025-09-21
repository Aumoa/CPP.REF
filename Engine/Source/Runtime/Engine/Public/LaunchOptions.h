// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommandLineParser.h"
#include "LaunchOptions.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API LaunchOptions : public Object
	{
		GENERATED_BODY()

	private:
		String m_Platform;
		std::unique_ptr<CommandLineParser> m_Args;

	public:
		LaunchOptions(String platform, std::unique_ptr<CommandLineParser> args);

		AFUNCTION()
		String GetPlatformAssemblyName() const;
		AFUNCTION()
		String GetGameAssemblyName() const;
	};
}