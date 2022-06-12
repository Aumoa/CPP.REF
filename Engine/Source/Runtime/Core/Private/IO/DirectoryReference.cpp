// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/DirectoryReference.h"
#include "IO/FileReference.h"

bool DirectoryReference::CreateIfNotExists(bool bRecursive) const noexcept
{
	if (!IsExists())
	{
		if (bRecursive)
		{
			std::error_code ec;
			return std::filesystem::create_directories(this->_Get_path(), ec);
		}
		else
		{
			std::error_code ec;
			return std::filesystem::create_directory(this->_Get_path(), ec);
		}
	}

	return true;
}

std::vector<FileReference> DirectoryReference::GetAllFiles(bool recursive) const noexcept
{
	if (!IsExists())
	{
		return {};
	}

	std::vector<FileReference> frs;

	if (recursive)
	{
		for (auto& file : std::filesystem::recursive_directory_iterator(this->_Get_path()))
		{
			if (!file.is_directory())
			{
				frs.emplace_back(FileReference(String(file.path().wstring())));
			}
		}
	}
	else
	{
		for (auto& file : std::filesystem::directory_iterator(this->_Get_path()))
		{
			if (!file.is_directory())
			{
				frs.emplace_back(FileReference(String(file.path().wstring())));
			}
		}
	}

	return frs;
}

FileReference DirectoryReference::GetFile(const String& filename) const noexcept
{
	return FileReference(String::Concat(GetPath(), TEXT("/"), filename));
}