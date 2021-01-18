// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Framework/Character.h"

class TH_API AMyCharacter : public ACharacter
{
public:
	using Super = ACharacter;
	using This = AMyCharacter;

public:
	AMyCharacter();
	~AMyCharacter();

	void Tick(Seconds deltaTime) override;
};