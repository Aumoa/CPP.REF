// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlateRHIRenderer.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "Application/SlateApplication.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericApplication.h"
#include "Widgets/SWindow.h"

NSlateRHIRenderer::NSlateRHIRenderer()
{
}

void NSlateRHIRenderer::BeginFrame()
{
	NRHIGlobal::GetDynamicRHI().BeginFrame();
}

void NSlateRHIRenderer::EndFrame()
{
	NRHIGlobal::GetDynamicRHI().EndFrame();
    NSlateApplication::Get().PresentAllWindows();
}

void NSlateRHIRenderer::FlushCommands()
{
}

void NSlateRHIRenderer::CreateViewport(SWindow& InWindow)
{
    NGenericWindowDefinition Def;
    Def.bPrimaryWindow = true;
    Def.bSystemMenu = true;
    Def.bThickframe = true;
    Def.bSizebox = true;
    Def.bCaption = true;
    std::shared_ptr GenericWindow = NGenericApplication::Get().MakeWindow(Def);
    InWindow.AttachWindow(std::move(GenericWindow));
}