// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "IO/FileReference.h"
#include "Object.h"
#include "LogCore.h"
#include "IO/DirectoryReference.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

FileReference::FileReference(const std::filesystem::path& filepath) : Super(filepath)
{
}

FileReference::~FileReference()
{
	FlushAndCloseSharedStream();
}

std::filesystem::path FileReference::GetFilename() const
{
	return GetPath().filename();
}

std::filesystem::path FileReference::GetName() const
{
	return GetPath().stem();
}

std::filesystem::path FileReference::GetExtension() const
{
	return GetPath().extension();
}

std::wfstream FileReference::OpenStream(std::ios_base::openmode mode, bool bCreateIfNotExists, bool bCreateDirectoryRecursive) const
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

std::wfstream& FileReference::OpenSharedStream(const Object* sharingUser, std::ios_base::openmode mode, bool bCreateIfNotExists, bool bCreateDirectoryRecursive)
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

void FileReference::CloseSharedStream(const Object* sharingUser)
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

void FileReference::FlushAndCloseSharedStream()
{
	if (_sharedstream.is_open())
	{
		_sharedstream.flush();
		_sharedstream.close();
	}
}