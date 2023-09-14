// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlateRHIRenderer.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHIViewport.h"
#include "RHI/RHICommandSet.h"
#include "RHI/RHICommandQueue.h"
#include "Application/SlateApplication.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericApplication.h"
#include "Widgets/SWindow.h"
#include "Rendering/SlateRenderProxy.h"

NSlateRHIRenderer::NSlateRHIRenderer()
{
}

NSlateRHIRenderer::~NSlateRHIRenderer() noexcept
{
}

void NSlateRHIRenderer::BeginFrame()
{
    NRHIGraphics& Graphics = NRHIGlobal::GetDynamicRHI();

    if (!CommandSet)
    {
        CommandSet = Graphics.CreateCommandSet();
    }

	Graphics.BeginFrame();
    CommandSet->BeginFrame();
}

void NSlateRHIRenderer::EndFrame()
{
    NRHIGraphics& Graphics = NRHIGlobal::GetDynamicRHI();

    CommandSet->EndFrame();

    NRHICommandSet* SetPtr = CommandSet.get();
    NRHIGlobal::GetPrimaryCommandQueue().ExecuteCommandSets({ &SetPtr, 1 });

    Graphics.EndFrame();
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

void NSlateRHIRenderer::BeginRender(const NRHIViewport& InViewport)
{
    CommandSet->BeginRender(InViewport, true);
}

void NSlateRHIRenderer::EndRender(const NRHIViewport& InViewport)
{
    CommandSet->EndRender(InViewport);
}

void NSlateRHIRenderer::RenderElement(const NSlateRenderProxy& InElement)
{
    PLATFORM_UNREFERENCED_PARAMETER(InElement);
}