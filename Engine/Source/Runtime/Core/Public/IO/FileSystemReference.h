// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <optional>
#include <filesystem>
#include <source_location>

namespace libty::inline Core
{
	class DirectoryReference;

	class CORE_API FileSystemReference
	{
	private:
		std::optional<std::filesystem::path> _path;

	public:
		FileSystemReference() = default;
		FileSystemReference(const FileSystemReference& rhs) = default;
		FileSystemReference(FileSystemReference&& rhs) = default;
		FileSystemReference(const std::filesystem::path& filepath);
		virtual ~FileSystemReference() noexcept = default;

		bool IsExists() const;
		std::filesystem::path GetPath() const;
		DirectoryReference GetParent() const;
		bool IsSet() const;

		FileSystemReference& operator =(const FileSystemReference& rhs) = default;
		FileSystemReference& operator =(FileSystemReference&& rhs) = default;

		auto operator <=>(const FileSystemReference&) const = default;
		bool operator ==(const FileSystemReference&) const = default;
	};
}