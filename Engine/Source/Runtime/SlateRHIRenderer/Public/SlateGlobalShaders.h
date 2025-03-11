// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHISlateShader;

struct SLATERHIRENDERER_API NSlateGlobalShaders : public StaticClass
{
	static void Initialize();
	static void Shutdown();

	static NRHISlateShader& GetSlateDefaultShader() { return *SlateDefaultShader; }

private:
	static std::shared_ptr<NRHISlateShader> SlateDefaultShader;
};