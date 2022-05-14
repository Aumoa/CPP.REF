// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileSystemReference.h"
#include "IO/DirectoryReference.h"

using namespace libty;

std::optional<DirectoryReference> FileSystemReference::GetParent() const
{
	auto p = this->_Get_path();
	if (p.has_parent_path())
	{
		return std::nullopt;
	}
	else
	{
		return DirectoryReference(String(p.parent_path().wstring()));
	}
}