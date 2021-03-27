// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "WindowsMinimal.h"

class GAME_API HandleBase : virtual public Object
{
public:
	using Super = Object;

private:
	HANDLE handle;

protected:
	bool bAutoCloseHandle : 1;

public:
	HandleBase();
	~HandleBase() override;

	virtual void Close();

	vs_property(HANDLE, Handle);
};