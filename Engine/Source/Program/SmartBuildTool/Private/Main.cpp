// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "BuildTool.h"

int wmain(int argc, wchar_t* argv[])
{
	std::vector<std::wstring_view> Args;
	Args.reserve((size_t)argc);

	for (size_t i = 0; i < (size_t)argc; ++i)
	{
		Args.emplace_back(argv[i]);
	}

	std::shared_ptr Tool = SObject::MakeShared<SBuildTool>();
	return Tool->GuardedMain(std::move(Args));
}