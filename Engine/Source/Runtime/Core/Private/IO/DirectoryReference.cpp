// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/DirectoryReference.h"
#include "IO/FileReference.h"

[[nodiscard]] std::vector<FileReference> DirectoryReference::GetFiles(bool bRecursive) const
{
	if (IsExists() == false)
	{
		return {};
	}

	ESearchOption Option = bRecursive ? ESearchOption::AllDirectories : ESearchOption::TopDirectoryOnly;
	return Directory::GetFiles(GetValue(), Option)
		| Linq::Select([](auto p) { return FileReference(p); })
		| Linq::ToVector();
}

[[nodiscard]] FileReference DirectoryReference::GetFile(String InName) const
{
	return Path::Combine(TrimPath(GetValue()), InName);
}