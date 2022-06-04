// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileSystemReference.h"
#include "IO/DirectoryReference.h"

std::optional<DirectoryReference> FileSystemReference::GetParent() const
{
	auto p = this->_Get_path();
	if (p.has_parent_path())
	{
		return DirectoryReference(String(p.parent_path().wstring()));
	}
	else
	{
		return std::nullopt;
	}
}