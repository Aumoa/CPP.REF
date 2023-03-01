// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/Path.h"
#include "CoreTypes/String.h"
#include <filesystem>

String Path::EngineDirectory;

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

String Path::Combine(std::span<const String> paths)
{
	std::filesystem::path r;
	for (auto& path : paths)
	{
		r /= path.c_str();
	}
	return String(std::move(r).wstring());
}

String Path::GetFullPath(String InPath)
{
	return String(std::filesystem::absolute(InPath.c_str()).wstring());
}

String Path::GetDirectoryName(String InPath)
{
	return String(std::filesystem::path(InPath.c_str()).parent_path().wstring());
}

void Path::SetEngineDirectory(String InDir)
{
	EngineDirectory = InDir;
}

String Path::GetEngineDirectory()
{
	return EngineDirectory;
}

String Path::GetEngineContentDirectory()
{
	return Path::Combine(EngineDirectory, TEXT("Content"));
}