// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

class String;

class CORE_API Path
{
public:
	static String GetFileNameWithoutExtension(const String& path);
	static bool HasExtension(const String& path);
	static String ChangeExtension(const String& path, const String& extension);
	static String GetRelativePath(const String& relativeTo, const String& path);
};