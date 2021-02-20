﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHIBundle.h"

class MaterialInterface;

interface GAME_API IRHIMaterialBundle : virtual public Object, virtual public IRHIBundle
{
	virtual TRefPtr<MaterialInterface> CreateMaterial() = 0;
	virtual uint64 GetMaterialsBufferVirtualAddress() const = 0;
	virtual MaterialInterface* FindMaterialFromIndex(uint16 index) const = 0;
};