// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "IO/Directory.h"

std::vector<String> Directory::GetFiles(String InPath, ESearchOption InSearchOption)
{
	std::vector<String> Files;

	if (InSearchOption == ESearchOption::TopDirectoryOnly)
	{
		for (auto It : std::filesystem::directory_iterator(InPath.path()))
		{
			Files.emplace_back(It.path().wstring());
		}
	}
	else
	{
		for (auto It : std::filesystem::recursive_directory_iterator(InPath.path()))
		{
			Files.emplace_back(It.path().wstring());
		}
	}

	return Files;
}

bool Directory::Exists(String InPath)
{
	return std::filesystem::is_directory(InPath.path());
}