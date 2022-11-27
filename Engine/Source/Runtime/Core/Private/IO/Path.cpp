// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/Path.h"
#include "Misc/String.h"
#include <filesystem>

String Path::GetFileName(const String& path)
{
	return String(std::filesystem::path((std::wstring_view)path).filename().wstring());
}

String Path::GetFileNameWithoutExtension(const String& path)
{
	return String(std::filesystem::path((std::wstring_view)path).stem().wstring());
}

bool Path::HasExtension(const String& path)
{
	return std::filesystem::path((const wchar_t*)path).has_extension();
}

String Path::ChangeExtension(const String& path, const String& extension)
{
	std::filesystem::path p((const wchar_t*)path);
	p.replace_extension(std::filesystem::path((const wchar_t*)extension));
	return String(p.wstring());
}

String Path::GetRelativePath(const String& relativeTo, const String& path)
{
	auto relativeTo_p = std::filesystem::path((std::wstring)relativeTo);
	auto path_p = std::filesystem::path((std::wstring)path);

	return String(std::filesystem::relative(relativeTo_p, path_p).wstring());
}