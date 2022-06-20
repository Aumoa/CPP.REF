// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/CommandLineBuilder.h"
#include "CommandLineBuilder.gen.cpp"

CommandLineBuilder::CommandLineBuilder(int32 argc, char** argv)
{
	for (size_t i = 0; i < (size_t)argc; ++i)
	{
		_args.emplace_back(String::FromLiteral(std::string_view(argv[i])));
	}
}