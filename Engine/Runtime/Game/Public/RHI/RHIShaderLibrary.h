// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIShader;

class GAME_API RHIShaderLibrary : virtual public Object
{
	std::map<size_t, TRefPtr<IRHIShader>> shaderMap;

public:
	RHIShaderLibrary();
	~RHIShaderLibrary() override;

	bool AddShader(TRefPtr<IRHIShader> shader);
	IRHIShader* GetShader(size_t shaderTypeHashCode) const;
};