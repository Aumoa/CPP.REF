// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FileSystemReference.h"
#include "Threading/Tasks/Task.h"

class CORE_API FileReference : public FileSystemReference
{
public:
	FileReference() = default;
	FileReference(const FileReference& rhs) = default;
	FileReference(FileReference&& rhs) = default;
	FileReference(String path) noexcept;

	String GetFilename() const noexcept;
	String GetExtension() const noexcept;

	String ReadAllText() const;
	bool WriteAllText(String Text, uint32 Encoding = 0) const;

	Task<String> ReadAllTextAsync(std::stop_token cancellationToken = {}) const;

	inline constexpr FileReference& operator =(const FileReference&) = default;
	inline constexpr FileReference& operator =(FileReference&&) = default;
};