// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlateRHIRenderer.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "Application/SlateApplication.h"

NSlateRHIRenderer::NSlateRHIRenderer()
{
}

void NSlateRHIRenderer::BeginFrame()
{
    auto* DynamicRHI = NRHIGlobal::GetDynamicRHI();
    DynamicRHI->BeginFrame();
}

void NSlateRHIRenderer::EndFrame()
{
    auto* DynamicRHI = NRHIGlobal::GetDynamicRHI();
    DynamicRHI->EndFrame();
    NSlateApplication::Get().PresentAllWindows();
}

void NSlateRHIRenderer::FlushCommands()
{
}