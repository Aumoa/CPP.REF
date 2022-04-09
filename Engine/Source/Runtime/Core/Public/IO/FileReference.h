// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "FileSystemReference.h"
#include <set>
#include <fstream>
#include <filesystem>

namespace libty::inline Core
{
	class CORE_API FileReference : public FileSystemReference
	{
	public:
		FileReference() = default;
		FileReference(const FileReference& rhs) = default;
		FileReference(FileReference&& rhs) = default;
		FileReference(const std::filesystem::path& filepath);

		std::filesystem::path GetFilename() const;
		std::filesystem::path GetName() const;
		std::filesystem::path GetExtension() const;

		std::wfstream OpenStream(std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out, bool bCreateIfNotExists = true, bool bCreateDirectoryRecursive = true) const;

		std::wstring ReadAllText();
		bool WriteAllText(std::wstring_view Text, uint32 Encoding = 0);

		FileReference& operator =(const FileReference& rhs) = default;
		FileReference& operator =(FileReference&& rhs) = default;

		auto operator <=>(const FileReference&) const = default;
		bool operator ==(const FileReference&) const = default;
	};
}