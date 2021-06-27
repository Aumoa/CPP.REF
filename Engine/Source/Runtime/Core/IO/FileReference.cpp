// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "FileReference.h"
#include "Object.h"
#include "LogCore.h"
#include "DirectoryReference.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

using enum ELogVerbosity;

using namespace std;

FileReference::FileReference(const filesystem::path& filepath) : Super(filepath)
{
}

FileReference::~FileReference()
{
	FlushAndCloseSharedStream();
}

filesystem::path FileReference::GetFilename() const
{
	return GetPath().filename();
}

filesystem::path FileReference::GetName() const
{
	return GetPath().stem();
}

filesystem::path FileReference::GetExtension() const
{
	return GetPath().extension();
}

wfstream FileReference::OpenStream(ios_base::openmode mode, bool bCreateIfNotExists, bool bCreateDirectoryRecursive) const
{
	if (bCreateIfNotExists && !IsExists())
	{
		if (bCreateDirectoryRecursive)
		{
			GetParent().CreateIfNotExists(bCreateDirectoryRecursive);
		}

		mode = ios_base::in | ios_base::out | ios_base::trunc;
	}

	return wfstream(GetPath(), mode);
}

wfstream& FileReference::OpenSharedStream(const Object* sharingUser, ios_base::openmode mode, bool bCreateIfNotExists, bool bCreateDirectoryRecursive)
{
	if (sharingUser == nullptr)
	{
		LogSystem::Log(LogCore, Error, L"The sharing user is nullptr. Abort.");
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
		LogSystem::Log(LogCore, Error, L"The sharing user is nullptr. Abort.");
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

	LogSystem::Log(LogCore, Verbose, L"The user {} is not contained from shared stream users.", sharingUser->ToString());
}

void FileReference::FlushAndCloseSharedStream()
{
	if (_sharedstream.is_open())
	{
		_sharedstream.flush();
		_sharedstream.close();
	}
}