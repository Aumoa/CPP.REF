// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHIShader;

struct NSlateGlobalShaders : public StaticClass
{
	static NRHIShader& GetSlateShader();

private:
	static std::shared_ptr<NRHIShader> pSlateShader;

private:
	static void InitializeShaders();
};