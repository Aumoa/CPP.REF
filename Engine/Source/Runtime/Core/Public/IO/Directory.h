// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

class String;

class CORE_API Directory
{
public:
	static bool TryCreateDirectory(const String& path);
	static bool Exists(const String& path);
};