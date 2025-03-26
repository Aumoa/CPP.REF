// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/DirectoryReference.h"
#include "IO/FileReference.h"

namespace Ayla
{
	[[nodiscard]] std::vector<FileReference> DirectoryReference::GetFiles(bool bRecursive) const
	{
		if (IsExists() == false)
		{
			return {};
		}

		SearchOption Option = bRecursive ? SearchOption::AllDirectories : SearchOption::TopDirectoryOnly;
		return Directory::GetFiles(GetValue(), Option)
			| Linq::Select([](auto p) { return FileReference(p); })
			| Linq::ToVector();
	}

	[[nodiscard]] FileReference DirectoryReference::GetFile(String InName) const
	{
		return Path::Combine(TrimPath(GetValue()), InName);
	}
}