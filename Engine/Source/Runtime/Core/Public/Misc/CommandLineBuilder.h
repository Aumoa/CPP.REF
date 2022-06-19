// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "CommandLineBuilder.generated.h"

SCLASS()
class CORE_API CommandLineBuilder : virtual public Object
{
	GENERATED_BODY()

private:
	std::vector<String> _args;

public:
	CommandLineBuilder(int32 argc, char** argv);

	inline size_t NumArguments() const noexcept { return _args.size(); }
	inline String GetArgument(size_t index) const noexcept { return _args[index]; }
};