// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/FileReference.h"
#include "IO/DirectoryReference.h"

namespace Ayla
{
	[[nodiscard]] FileReference FileReference::WithExtensions(String InExtensions) const
	{
		if (InExtensions.IsEmpty())
		{
			return Path::Combine((String)GetDirectory(), GetName());
		}

		return Path::Combine((String)GetDirectory(), String::Format(TEXT("{0}.{1}"), GetName(), InExtensions));
	}

	DirectoryReference FileReference::GetDirectory() const
	{
		String DirectoryName = Path::GetDirectoryName(GetValue());
		return DirectoryReference(DirectoryName);
	}
}