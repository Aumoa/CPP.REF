// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "HandleBase.h"

class GAME_API WinEvent : public HandleBase
{
public:
	using Super = HandleBase;

public:
	WinEvent(HANDLE hEvent);
	WinEvent(bool bManual = false, bool bInitialSet = false, TRefPtr<String> eventName = nullptr);
	~WinEvent() override;
	
	bool Wait(std::optional<Milliseconds> timeout = std::nullopt);

	static TRefPtr<WinEvent> OpenEvent(TRefPtr<String> eventName);
};