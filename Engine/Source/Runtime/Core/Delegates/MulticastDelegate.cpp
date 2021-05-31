// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;

DelegateHandle::DelegateHandle()
{
}

DelegateHandle::~DelegateHandle()
{
	Unlink();
}

void DelegateHandle::Unlink() noexcept
{
	if (IsValid())
	{
		_remove();
		_remove = nullptr;
	}
}

bool DelegateHandle::IsValid() const
{
	return (bool)_remove;
}