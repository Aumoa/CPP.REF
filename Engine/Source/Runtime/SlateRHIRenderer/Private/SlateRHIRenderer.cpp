// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SlateRHIRenderer.h"
#include "SlateGlobalShaders.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHIViewport.h"
#include "RHI/RHICommandSet.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIStructuredBuffer.h"
#include "RHI/RHIDescriptorHeap.h"
#include "Application/SlateApplication.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericApplication.h"
#include "Widgets/SWindow.h"
#include "Numerics/VectorInterface/Color.h"

NSlateRHIRenderer::NSlateRHIRenderer()
{
}

NSlateRHIRenderer::~NSlateRHIRenderer() noexcept
{
}

void NSlateRHIRenderer::BeginFrame()
{
    VpIndex = 0;
}

void NSlateRHIRenderer::EndFrame()
{
    size_t NumActualVps = VpIndex;
    std::vector<NRHICommandSet*> Sets;
    Sets.reserve(NumActualVps);

    for (size_t i = 0; i < NumActualVps; ++i)
    {
        Sets.emplace_back(CachedVpCommands[i].CommandSet.get());
    }

    NRHIGlobal::GetPrimaryCommandQueue().ExecuteCommandSets(Sets);
}

void NSlateRHIRenderer::FlushCommands()
{
    NRHIGlobal::GetDynamicRHI().SyncFrame();
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
        pVpCommands->DescriptorHeap = Graphics.CreateDescriptorHeap();
    }

    pVpCommands->Restart();
    pVpCommands->CommandSet->BeginFrame();
    pVpCommands->CommandSet->BeginRender(InViewport, true);

    pVpCommands->CommandSet->SetSlateShader(NSlateGlobalShaders::GetSlateDefaultShader());
    pVpCommands->CommandSet->SetScreenResolutionInfo(Vector<>::Cast<float>(InViewport.GetViewportSize()));
}

void NSlateRHIRenderer::EndRender(const NRHIViewport& InViewport)
{
    NViewportCommands& VpCommands = CachedVpCommands[VpIndex++];
    VpCommands.CommandSet->EndRender(InViewport);
    VpCommands.CommandSet->EndFrame();
}

void NSlateRHIRenderer::DrawLayered(const NSlateWindowElementList& InElementList)
{
    if (InElementList.UnorderedElements.empty())
    {
        return;
    }

    NViewportCommands& VpCommands = CachedVpCommands[VpIndex];

    VpCommands.Layers.clear();
    const size_t NumElements = InElementList.UnorderedElements.size();
    const size_t PaintGeometrySize = sizeof(NSlateShaderPaintGeometry) * NumElements;
    const size_t ShaderRenderParamsSize = sizeof(NSlateShaderRenderParams) * NumElements;
    const size_t TotalSize = PaintGeometrySize + ShaderRenderParamsSize;

    if (VpCommands.StructuredBuffers == nullptr || VpCommands.StructuredBuffers->GetBufferSize() < TotalSize)
    {
        VpCommands.StructuredBuffers = NRHIGlobal::GetDynamicRHI().CreateStructuredBuffer(TotalSize);
    }

    int8* pHeap = (int8*)VpCommands.StructuredBuffers->GetBufferPointer();
    auto* pPaintGeometry = (NSlateShaderPaintGeometry*)(pHeap + 0);
    auto* pRenderParams = (NSlateShaderRenderParams*)(pHeap + PaintGeometrySize);
    int32 TotalDescriptors = 0;

    int64 VirtualAddr = VpCommands.StructuredBuffers->GetGPUVirtualAddress();
    int64 VPaintGeometry = VirtualAddr + 0;
    int64 VRenderParams = VirtualAddr + PaintGeometrySize;

    bool bNewLayer = false;
    NViewportCommands::NLayered* pLayered = nullptr;

    int32 CurrentIndex = 0;

    for (auto& Element : InElementList.UnorderedElements)
    {
        NSlateShaderPaintGeometry* pCurrentPaintGeometry = pPaintGeometry + CurrentIndex;
        NSlateShaderRenderParams* pCurrentRenderParams = pRenderParams + CurrentIndex;

        if (Element.Layout.HasRenderTransform())
        {
            auto& RT = Element.Layout.GetAccumulatedRenderTransform();
            pCurrentPaintGeometry->Transformation = RT.M;
            pCurrentPaintGeometry->Translation = RT.Translation;
        }
        else
        {
            pCurrentPaintGeometry->Transformation = Matrix2x2::Identity();
            pCurrentPaintGeometry->Translation = Element.AbsolutePosition;
        }
        pCurrentPaintGeometry->LocalSize = Element.Layout.GetLocalSize();

        pCurrentRenderParams->TintColor = Element.TintColor;
        pCurrentRenderParams->RenderOpacity = Element.RenderOpacity;
        pCurrentRenderParams->RenderStates = 0;

        if (VpCommands.Layers.empty() || VpCommands.Layers.back().Layer != Element.Layer)
        {
            bNewLayer = true;
        }

        NRHIShaderResourceView* pSRV = nullptr;
        if (Element.Proxy && Element.Proxy->TryResolve())
        {
            pCurrentPaintGeometry->TextureCoordinate = Element.Proxy->GetTextureCoordinate();
            pCurrentRenderParams->RenderStates = Element.Proxy->GetRenderStates();
            pSRV = Element.Proxy->GetSRV().get();
            if (pSRV)
            {
                ++TotalDescriptors;
            }

            if (VpCommands.Layers.empty() || VpCommands.Layers.back().pSRV != pSRV)
            {
                bNewLayer = true;
            }
        }

        if (bNewLayer)
        {
            pLayered = &VpCommands.Layers.emplace_back();
            pLayered->Layer = Element.Layer;
            pLayered->pSRV = pSRV;
            pLayered->VPaintGeometryStart = VPaintGeometry + sizeof(NSlateShaderPaintGeometry) * CurrentIndex;
            pLayered->VRenderParamsStart = VRenderParams + sizeof(NSlateShaderRenderParams) * CurrentIndex;
        }

        check(pLayered);
        ++pLayered->NumElements;
        ++CurrentIndex;
    }

    if (TotalDescriptors)
    {
        VpCommands.DescriptorHeap->Reserve(TotalDescriptors);

        NRHIDescriptorHeap* Heaps[]{ VpCommands.DescriptorHeap.get() };
        VpCommands.CommandSet->SetDescriptorHeaps(Heaps);

        size_t Index = 0;
        for (auto& Layered : VpCommands.Layers)
        {
            if (Layered.pSRV)
            {
                Layered.VSRVHandle = VpCommands.DescriptorHeap->GetVirtualHandleLocation(Index);
                VpCommands.DescriptorHeap->ApplyViewSimple(Index, *Layered.pSRV, 0, 1);
                ++Index;
            }
        }
    }

    std::ignore = VpCommands.StructuredBuffers->CommitAsync();

    for (auto& Layered : VpCommands.Layers)
    {
        VpCommands.CommandSet->SetPaintGeometry(Layered.VPaintGeometryStart);
        VpCommands.CommandSet->SetRenderParams(Layered.VRenderParamsStart);
        if (Layered.pSRV)
        {
            VpCommands.CommandSet->SetSlateInputTexture(Layered.VSRVHandle);
        }
        VpCommands.CommandSet->DrawSlateInstance(Layered.NumElements);
    }
}