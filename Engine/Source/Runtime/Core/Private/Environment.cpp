// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Environment.h"
#include "Environment.gen.cpp"

String Environment::GetCurrentDirectory()
{
	return String(std::filesystem::current_path().wstring());
}