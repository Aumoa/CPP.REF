// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "IO/FileReference.h"
#include "Object.h"
#include "LogCore.h"
#include "IO/DirectoryReference.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

SFileReference::SFileReference(const std::filesystem::path& filepath) : Super(filepath)
{
}

SFileReference::~SFileReference()
{
	FlushAndCloseSharedStream();
}

std::filesystem::path SFileReference::GetFilename() const
{
	return GetPath().filename();
}

std::filesystem::path SFileReference::GetName() const
{
	return GetPath().stem();
}

std::filesystem::path SFileReference::GetExtension() const
{
	return GetPath().extension();
}

std::wfstream SFileReference::OpenStream(std::ios_base::openmode mode, bool bCreateIfNotExists, bool bCreateDirectoryRecursive) const
{
	if (bCreateIfNotExists && !IsExists())
	{
		if (bCreateDirectoryRecursive)
		{
			GetParent().CreateIfNotExists(bCreateDirectoryRecursive);
		}

		mode = std::ios_base::in | std::ios_base::out | std::ios_base::trunc;
	}

	return std::wfstream(GetPath(), mode);
}

std::wfstream& SFileReference::OpenSharedStream(SObject* sharingUser, std::ios_base::openmode mode, bool bCreateIfNotExists, bool bCreateDirectoryRecursive)
{
	if (sharingUser == nullptr)
	{
		LogSystem::Log(LogCore, ELogVerbosity::Error, L"The sharing user is nullptr. Abort.");
		return _sharedstream;
	}

	if (_shared.size() == 0)
	{
		_sharedstream = OpenStream(mode, bCreateIfNotExists, bCreateDirectoryRecursive);
	}

	auto it = _shared.find(sharingUser);
	if (it == _shared.end())
	{
		_shared.emplace(sharingUser);
	}

	return _sharedstream;
}

void SFileReference::CloseSharedStream(SObject* sharingUser)
{
	if (sharingUser == nullptr)
	{
		LogSystem::Log(LogCore, ELogVerbosity::Error, L"The sharing user is nullptr. Abort.");
		return;
	}

	if (auto it = _shared.find(sharingUser); it != _shared.end())
	{
		_shared.erase(it);

		if (_shared.size() == 0)
		{
			FlushAndCloseSharedStream();
		}

		return;
	}

	LogSystem::Log(LogCore, ELogVerbosity::Verbose, L"The user {} is not contained from shared stream users.", sharingUser->ToString());
}

std::string SFileReference::ReadAllText()
{
	std::ifstream File(GetPath());
	if (File.is_open())
	{
		std::string Buf;
		Buf.resize(File.seekg(0, std::ios::end).tellg());
		File.seekg(0, std::ios::beg);

		if (File.read(Buf.data(), Buf.size()).bad())
		{
			File.close();
			return "";
		}

		auto Rit = Buf.rbegin();
		for (; Rit != Buf.rend(); ++Rit)
		{
			if (*Rit != 0)
			{
				break;
			}
		}

		Buf.erase(Rit.base(), Buf.end());

		File.close();
		return Buf;
	}
	return "";
}

bool SFileReference::WriteAllText(std::string_view Text)
{
	std::ofstream File(GetPath(), std::ios::trunc);
	if (File.is_open())
	{
		File << Text;
		File.close();
		return true;
	}
	return false;
}

void SFileReference::FlushAndCloseSharedStream()
{
	if (_sharedstream.is_open())
	{
		_sharedstream.flush();
		_sharedstream.close();
	}
}