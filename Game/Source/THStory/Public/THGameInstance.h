// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "GameInstance.h"

class THSTORY_API THGameInstance : public GameInstance
{
	GENERATED_BODY(THGameInstance)

public:
	THGameInstance();

	virtual void Init() override;
	virtual std::wstring GetApplicationName() const override { return L"THStory"; }
};