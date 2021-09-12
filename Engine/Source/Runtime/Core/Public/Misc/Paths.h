// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <filesystem>

class CORE_API Paths abstract final
{
public:
	static std::filesystem::path GetContentPath();
	static std::filesystem::path GetPlatformFontPath();
};