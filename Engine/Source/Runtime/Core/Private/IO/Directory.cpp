// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/Directory.h"

std::vector<String> Directory::GetFiles(String InPath, ESearchOption InSearchOption)
{
	std::vector<String> Files;

	if (InSearchOption == ESearchOption::TopDirectoryOnly)
	{
		for (auto It : std::filesystem::directory_iterator(InPath.path()))
		{
			if (std::filesystem::is_regular_file(It.status()))
			{
				Files.emplace_back(It.path().wstring());
			}
		}
	}
	else
	{
		for (auto It : std::filesystem::recursive_directory_iterator(InPath.path()))
		{
			if (std::filesystem::is_regular_file(It.status()))
			{
				Files.emplace_back(It.path().wstring());
			}
		}
	}

	return Files;
}

std::vector<String> Directory::GetDirectories(String InPath, ESearchOption InSearchOption)
{
	std::vector<String> Files;

	if (InSearchOption == ESearchOption::TopDirectoryOnly)
	{
		for (auto It : std::filesystem::directory_iterator(InPath.path()))
		{
			if (std::filesystem::is_directory(It.status()))
			{
				Files.emplace_back(It.path().wstring());
			}
		}
	}
	else
	{
		for (auto It : std::filesystem::recursive_directory_iterator(InPath.path()))
		{
			if (std::filesystem::is_directory(It.status()))
			{
				Files.emplace_back(It.path().wstring());
			}
		}
	}

	return Files;
}

bool Directory::Exists(String InPath)
{
	return std::filesystem::is_directory(InPath.path());
}

bool Directory::CreateDirectory(String InPath)
{
	return std::filesystem::create_directories(InPath.path());
}

void Directory::Delete(String InPath, bool bRecursive)
{
	if (bRecursive)
	{
		std::filesystem::remove_all(InPath.path());
	}
	else
	{
		std::filesystem::remove(InPath.path());
	}
}