// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/StaticClass.h"
#include "CoreTypes/String.h"
#include <span>
#include <initializer_list>
#include <vector>

class CORE_API Path : public StaticClass
{
private:
	static String EngineDirectory;

public:
	static String GetFileName(const String& path);
	static String GetFileNameWithoutExtension(const String& path);
	static bool HasExtension(const String& path);
	static String ChangeExtension(const String& path, const String& extension);
	static String GetRelativePath(const String& relativeTo, const String& path);
	static String Combine(std::span<const String> paths);
	static String GetFullPath(String InPath);
	static String GetDirectoryName(String InPath);

	static void SetEngineDirectory(String InDir);
	static String GetEngineDirectory();
	static String GetEngineContentDirectory();

public:
	template<class... T>
	static String Combine(T&&... paths) requires requires { { std::initializer_list<String>{ paths... } }; }
	{
		std::vector<String> v{ std::forward<T>(paths)... };
		return Combine(v);
	}
};