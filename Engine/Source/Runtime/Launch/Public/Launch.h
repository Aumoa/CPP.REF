// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NEngineLoop;

class LAUNCH_API NLaunch
{
private:
	String CmdArgs;
    std::unique_ptr<NEngineLoop> Loop;

protected:
	NLaunch(String CmdArgs);

public:
	virtual ~NLaunch() noexcept;

public:
	int32 GuardedMain();

	static std::unique_ptr<NLaunch> GeneratePlatformLaunch(String CmdArgs);
	static std::unique_ptr<NLaunch> GeneratePlatformLaunch(int Argc, char** Argv)
	{
		std::vector<String> Args;
		for (int i = 0; i < Argc; ++i)
		{
			Args.emplace_back(String::FromLiteral(Argv[i]));
		}
		return GeneratePlatformLaunch(String::Join(TEXT(" "), Args));
	}
};