// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngine.h"

class COREED_API EditorEngine : public GameEngine
{
public:
	EditorEngine();
	virtual ~EditorEngine() noexcept override;
};