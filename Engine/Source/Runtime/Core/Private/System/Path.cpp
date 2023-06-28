// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;

String Path::EngineDirectory;

String Path::GetFileName(const String& path)
{
	return String(std::filesystem::path((std::wstring_view)path).filename().c_str());
}

String Path::GetFileNameWithoutExtension(const String& path)
{
	return String(std::filesystem::path((std::wstring_view)path).stem().c_str());
}

bool Path::HasExtension(const String& path)
{
	return std::filesystem::path((const wchar_t*)path).has_extension();
}

String Path::ChangeExtension(const String& path, const String& extension)
{
	std::filesystem::path p((const wchar_t*)path);
	p.replace_extension(std::filesystem::path((const wchar_t*)extension));
	return String(p.c_str());
}

String Path::GetRelativePath(const String& relativeTo, const String& path)
{
	auto relativeTo_p = std::filesystem::path((std::wstring)relativeTo);
	auto path_p = std::filesystem::path((std::wstring)path);

	return String(std::filesystem::relative(relativeTo_p, path_p).c_str());
}

String Path::Combine(std::span<const String> paths)
{
	std::filesystem::path r;
	for (auto& path : paths)
	{
		r /= path.c_str();
	}
	return String(std::move(r).c_str());
}

String Path::GetFullPath(String InPath)
{
	return String(std::filesystem::absolute(InPath.c_str()).c_str());
}

String Path::GetDirectoryName(String InPath)
{
	return String(std::filesystem::path(InPath.c_str()).parent_path().c_str());
}

String Path::GetExtension(String InPath)
{
	return String(std::filesystem::path(InPath.c_str()).extension().c_str());
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