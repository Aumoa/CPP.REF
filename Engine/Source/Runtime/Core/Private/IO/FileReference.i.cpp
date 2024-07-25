// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "System/LanguageSupportMacros.h"

import Core;

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