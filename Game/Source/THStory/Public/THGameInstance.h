// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance.h"

class THSTORY_API STHGameInstance : public SGameInstance
{
	GENERATED_BODY(STHGameInstance)

public:
	STHGameInstance();

	virtual void Init() override;
	virtual std::wstring GetApplicationName() override { return L"THStory"; }
};