// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/Directory.h"
#include "CoreTypes/String.h"
#include "Exceptions/SystemException.h"
#include <filesystem>

bool Directory::TryCreateDirectory(String InPath, std::error_code* OutErrorCode)
{
	std::error_code ec_;
	bool success = std::filesystem::create_directories((std::wstring)InPath, ec_);
	if (OutErrorCode)
	{
		if (success)
		{
			OutErrorCode->clear();
		}
		else
		{
			*OutErrorCode = ec_;
		}
	}

	return success;
}

void Directory::CreateDirectory(String InPath)
{
	std::error_code ec_;
	if (Exists(InPath) == false)
	{
		if (std::filesystem::create_directories((std::wstring)InPath, ec_) == false)
		{
			throw SystemException(ec_);
		}
	}
}

bool Directory::Exists(String InPath)
{
	std::error_code ec;
	return std::filesystem::is_directory((std::wstring)InPath, ec);
}

std::vector<String> Directory::GetFiles(String InPath, EDirectorySearchOptions Options)
{
	namespace fs = std::filesystem;

	std::vector<String> Entries;
	auto AddEntry = [&Entries](const std::filesystem::path& p)
	{
		Entries.emplace_back(String(p.c_str()));
	};

	if (Options == EDirectorySearchOptions::TopDirectoryOnly)
	{
		for (auto& Entry : fs::directory_iterator(InPath.c_str()))
		{
			if (Entry.is_regular_file())
			{
				AddEntry(Entry.path());
			}
		}
	}
	else
	{
		for (auto& Entry : fs::recursive_directory_iterator(InPath.c_str()))
		{
			if (Entry.is_regular_file())
			{
				AddEntry(Entry.path());
			}
		}
	}

	return Entries;
}