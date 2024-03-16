// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Params.h"

DirectoryReference Params::s_InputDirectory;

void Params::Init()
{
	static constexpr String k_Input = TEXT("Input");

	String value;
	if (CommandLine::TryGetValue(k_Input, value) == false || value.IsEmpty())
	{
		throw ArgumentException(k_Input);
	}
	s_InputDirectory = value;
}

DirectoryReference Params::GetInputDirectory()
{
	return s_InputDirectory;
}