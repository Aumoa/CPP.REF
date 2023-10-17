// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SlateRHIRenderer.h"
#include "SlateGlobalShaders.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHIViewport.h"
#include "RHI/RHICommandSet.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIConstantBuffer.h"
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
        pVpCommands->ConstantBuffers = Graphics.CreateConstantBuffer();
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

template<class T>
inline T* GetConstantBufferPtr(std::shared_ptr<NRHIConstantBuffer>& ConstantBuffer, size_t& Usage, int64& VirtualLocation)
{
    auto* Vp = (uint8*)ConstantBuffer->GetBufferPointer();
    Vp += Usage;
    VirtualLocation = ConstantBuffer->GetGPUVirtualAddress();
    VirtualLocation += Usage;
    Usage += sizeof(T);
    return (T*)Vp;
}

void NSlateRHIRenderer::RenderElement(const NSlateRenderElement& InElement)
{
    NViewportCommands& VpCommands = CachedVpCommands[VpIndex];

    int64 VL_PaintGeometry;
    auto* CB_PaintGeometry = GetConstantBufferPtr<NSlateShaderPaintGeometry>(VpCommands.ConstantBuffers, VpCommands.ConstantBufferUsage, VL_PaintGeometry);
    if (InElement.Layout.HasRenderTransform())
    {
        CB_PaintGeometry->Transformation = InElement.Layout.GetAccumulatedRenderTransform().M;
        CB_PaintGeometry->Translation = InElement.Layout.GetAccumulatedRenderTransform().Translation;
    }
    else
    {
        CB_PaintGeometry->Transformation = Matrix2x2::Identity();
        CB_PaintGeometry->Translation = InElement.AbsolutePosition;
    }
    CB_PaintGeometry->LocalSize = InElement.Layout.GetLocalSize();

    int64 VL_RenderParams;
    auto* CB_RenderParams = GetConstantBufferPtr<NSlateShaderRenderParams>(VpCommands.ConstantBuffers, VpCommands.ConstantBufferUsage, VL_RenderParams);
    CB_RenderParams->TintColor = InElement.TintColor;
    CB_RenderParams->RenderOpacity = InElement.RenderOpacity;
    CB_RenderParams->RenderStates = 0;

    if (InElement.Proxy && InElement.Proxy->TryResolve())
    {
        CB_PaintGeometry->TextureCoordinate = InElement.Proxy->GetTextureCoordinate();
        CB_RenderParams->RenderStates = InElement.Proxy->GetRenderStates();
        VpCommands.DescriptorHeap->ApplyViewSimple(VpCommands.DescriptorUsage, *InElement.Proxy->GetSRV(), 0, 1);
        int64 VirtualHandleLocation = VpCommands.DescriptorHeap->GetVirtualHandleLocation(VpCommands.DescriptorUsage++);
        VpCommands.CommandSet->SetSlateInputTexture(VirtualHandleLocation);
    }

    VpCommands.CommandSet->SetPaintGeometry(VL_PaintGeometry);
    VpCommands.CommandSet->SetRenderParams(VL_RenderParams);
    VpCommands.CommandSet->DrawSlateInstance();
}

void NSlateRHIRenderer::Populate(const NSlateWindowElementList& InElementList)
{
    NViewportCommands& VpCommands = CachedVpCommands[VpIndex];
    size_t NumElements = InElementList.UnorderedElements.size();
    size_t ConstantBufferSize
        = sizeof(NSlateShaderPaintGeometry) * NumElements
        + sizeof(NSlateShaderRenderParams) * NumElements
        ;
    VpCommands.ConstantBuffers->Reserve(ConstantBufferSize);

    size_t NumDescriptors = 0;
    for (auto& Element : InElementList.UnorderedElements)
    {
        if (Element.Proxy)
        {
            ++NumDescriptors;
        }
    }

    if (NumDescriptors)
    {
        VpCommands.DescriptorHeap->Reserve(NumDescriptors);

        NRHIDescriptorHeap* Heaps[]{ VpCommands.DescriptorHeap.get() };
        VpCommands.CommandSet->SetDescriptorHeaps(Heaps);
    }
}