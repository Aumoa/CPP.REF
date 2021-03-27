// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Windows/HandleBase.h"

HandleBase::HandleBase() : Super()
	, bAutoCloseHandle(false)
	, handle(nullptr)
{

}

HandleBase::~HandleBase()
{
	if (bAutoCloseHandle && handle)
	{
		Close();
	}
}

void HandleBase::Close()
{
	if (handle != NULL)
	{
		CloseHandle(handle);
		handle = nullptr;
	}
}

HANDLE HandleBase::Handle_get() const
{
	return handle;
}

void HandleBase::Handle_set(HANDLE value)
{
	if (handle == value)
	{
		return;
	}

	HANDLE prev = Handle;

	if (bAutoCloseHandle && prev != nullptr)
	{
		Close();
	}

	handle = value;
}