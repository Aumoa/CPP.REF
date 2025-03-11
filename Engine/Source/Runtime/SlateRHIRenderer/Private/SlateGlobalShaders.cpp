// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SlateGlobalShaders.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHISlateShader.h"

std::shared_ptr<NRHISlateShader> NSlateGlobalShaders::SlateDefaultShader;

void NSlateGlobalShaders::Initialize()
{
	auto& Graphics = NRHIGlobal::GetDynamicRHI();
	SlateDefaultShader = Graphics.CreateSlateShader();
}

void NSlateGlobalShaders::Shutdown()
{
	SlateDefaultShader.reset();
}