// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"

class FileReference : public FileSystemReference
{
public:
	inline constexpr FileReference() noexcept
	{
	}

	inline constexpr FileReference(const FileReference& rhs) noexcept
		: FileSystemReference(rhs)
	{
	}

	inline constexpr FileReference(FileReference&& rhs) noexcept
		: FileSystemReference(std::move(rhs))
	{
	}

	inline constexpr FileReference(String path) noexcept
		: FileSystemReference(path)
	{
	}

	inline String GetFilename() const noexcept
	{
		return String(this->_Get_path().filename().wstring());
	}

	inline String GetExtension() const noexcept
	{
		return String(this->_Get_path().extension().wstring());
	}

	CORE_API String ReadAllText() const;
	CORE_API bool WriteAllText(String Text, uint32 Encoding = 0) const;

	inline constexpr FileReference& operator =(const FileReference&) = default;
	inline constexpr FileReference& operator =(FileReference&&) = default;
};