// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SlateGlobalShaders.h"
#include "RHI/RHIShader.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

std::shared_ptr<NRHIShader> NSlateGlobalShaders::pSlateShader;

NRHIShader& NSlateGlobalShaders::GetSlateShader()
{
	InitializeShaders();
	return *pSlateShader;
}

void NSlateGlobalShaders::InitializeShaders()
{
	static int InitTrap = ([&]()
	{
		pSlateShader = NRHIGlobal::GetDynamicRHI().CreateShader();
	}(), 0);
}