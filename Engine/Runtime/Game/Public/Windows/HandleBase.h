// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "WindowsMinimal.h"

class GAME_API HandleBase : virtual public Object
{
public:
	using Super = Object;

protected:
	bool bAutoCloseHandle : 1;
	HANDLE handle;

public:
	HandleBase();
	~HandleBase() override;

	virtual void Close();

	vs_property(HANDLE, Handle);
	HANDLE Handle_get() const;

protected:
	void Handle_set(HANDLE value);
};