// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHIShader;

class GAME_API RHIShaderLibrary : virtual public Object
{
public:
	using Super = Object;
	using This = RHIShaderLibrary;

public:
	enum : size_t
	{
		GeometryShader,
		LightingShader,
		TonemapShader
	};

	std::vector<TRefPtr<IRHIShader>> shaders;

public:
	RHIShaderLibrary();
	~RHIShaderLibrary() override;

	void SetShader(size_t inShaderIndex, TRefPtr<IRHIShader> inShader);
	IRHIShader* GetShader(size_t inShaderIndex) const;
};