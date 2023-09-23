// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlateRHIRenderer.h"
#include "SlateGlobalShaders.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHIViewport.h"
#include "RHI/RHICommandSet.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIConstantBuffer.h"
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
    NRHIGlobal::GetDynamicRHI().BeginFrame();
    VpIndex = 0;
}

void NSlateRHIRenderer::EndFrame()
{
    NRHIGraphics& Graphics = NRHIGlobal::GetDynamicRHI();

    size_t NumActualVps = VpIndex;
    std::vector<NRHICommandSet*> Sets;
    Sets.reserve(NumActualVps);

    for (size_t i = 0; i < NumActualVps; ++i)
    {
        Sets.emplace_back(CachedVpCommands[i].CommandSet.get());
    }

    NRHIGlobal::GetPrimaryCommandQueue().ExecuteCommandSets(Sets);

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
    NViewportCommands* pVpCommands;
    if (CachedVpCommands | Linq::IsValidIndex(VpIndex))
    {
        pVpCommands = &CachedVpCommands[VpIndex];
    }
    else
    {
        NRHIGraphics& Graphics = NRHIGlobal::GetDynamicRHI();

        pVpCommands = &CachedVpCommands.emplace_back();
        pVpCommands->CommandSet = Graphics.CreateCommandSet();
        pVpCommands->ConstantBuffers = Graphics.CreateConstantBuffer();
    }

    pVpCommands->ConstantBufferUsage = 0;

    pVpCommands->CommandSet->BeginFrame(&NSlateGlobalShaders::GetSlatePipelineState());
    pVpCommands->CommandSet->SetGraphicsRootSignature(NSlateGlobalShaders::GetSlateRootSignature());
    pVpCommands->CommandSet->BeginRender(InViewport, true);

    Vector2 ScreenResolution = Vector<>::Cast<float>(InViewport.GetViewportSize());
    pVpCommands->CommandSet->SetGraphicsRoot32BitConstants(0, 2, &ScreenResolution, 0);
}

void NSlateRHIRenderer::EndRender(const NRHIViewport& InViewport)
{
    NViewportCommands& VpCommands = CachedVpCommands[VpIndex++];
    VpCommands.CommandSet->EndRender(InViewport);
    VpCommands.CommandSet->EndFrame();
}

struct alignas(256) NCV_SlatePaintGeometry
{
    Matrix2x2 Transformation;
    Vector2 Translation;
    Vector2 LocalSize;
};

void NSlateRHIRenderer::RenderElement(const NSlateRenderElement& InElement)
{
    PLATFORM_UNREFERENCED_PARAMETER(InElement);
    NViewportCommands& VpCommands = CachedVpCommands[VpIndex];

    auto* Cvp = (NCV_SlatePaintGeometry*)((uint8*)VpCommands.ConstantBuffers->GetBufferPointer() + VpCommands.ConstantBufferUsage);
    if (InElement.Layout.HasRenderTransform())
    {
        Cvp->Transformation = InElement.Layout.GetAccumulatedRenderTransform().M;
        Cvp->Translation = InElement.Layout.GetAccumulatedRenderTransform().Translation;
    }
    else
    {
        Cvp->Transformation = Matrix2x2::Identity();
        Cvp->Translation = InElement.AbsolutePosition;
    }
    Cvp->LocalSize = InElement.Layout.GetLocalSize();

    int64 BufferLocation = VpCommands.ConstantBuffers->GetGPUVirtualAddress();
    BufferLocation += VpCommands.ConstantBufferUsage;
    VpCommands.CommandSet->SetGraphicsRootConstantBufferView(1, BufferLocation);
    VpCommands.CommandSet->DrawInstanced(true, 4, 1, 0, 0);

    VpCommands.ConstantBufferUsage += sizeof(NCV_SlatePaintGeometry);
}

void NSlateRHIRenderer::Populate(const NSlateWindowElementList& InElementList)
{
    NViewportCommands& VpCommands = CachedVpCommands[VpIndex];
    size_t NumElements = InElementList.UnorderedElements.size();
    size_t ConstantBufferSize = sizeof(NCV_SlatePaintGeometry) * NumElements;
    VpCommands.ConstantBuffers->Reserve(ConstantBufferSize);
}