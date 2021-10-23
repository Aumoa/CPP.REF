// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Level/Level.h"

class THSTORY_API SStartupLevel : public SLevel
{
	GENERATED_BODY(SStartupLevel)

public:
	SStartupLevel();

protected:
	virtual void OnLoadLevel() override;
};