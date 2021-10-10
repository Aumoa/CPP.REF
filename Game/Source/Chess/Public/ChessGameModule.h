// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModule.h"

class CHESS_API SChessGameModule : public SGameModule
{
	GENERATED_BODY(SChessGameModule)

public:
	virtual SGameInstance* CreatePrimaryGameModule() override;
};