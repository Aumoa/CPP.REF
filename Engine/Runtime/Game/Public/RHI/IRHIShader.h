// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIShader : virtual public Object
{
	virtual size_t GetShaderTypeHashCode() const = 0;
};