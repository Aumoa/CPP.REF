// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <vector>

class CORE_API CommandLineBuilder
{
private:
	std::vector<String> _args;

public:
	CommandLineBuilder(int32 argc, char** argv);
	CommandLineBuilder(char* args);

	inline size_t NumArguments() const noexcept { return _args.size(); }
	inline String GetArgument(size_t index) const noexcept { return _args[index]; }
};