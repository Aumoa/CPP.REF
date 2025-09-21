// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "LaunchOptions.h"

namespace Ayla
{
	LaunchOptions::LaunchOptions(String platform, std::unique_ptr<CommandLineParser> args)
		: m_Platform(platform)
		, m_Args(std::move(args))
	{
	}

	String LaunchOptions::GetPlatformAssemblyName() const
	{
		return m_Platform;
	}

	String LaunchOptions::GetGameAssemblyName() const
	{
		auto& options = m_Args->Options();
		auto it = options.find(TEXT("gameassembly"));
		if (it == options.end())
		{
			throw InvalidOperationException(TEXT("No game assembly specified."));
		}
		return Path::GetFileName(it->second[0].value());
	}
}