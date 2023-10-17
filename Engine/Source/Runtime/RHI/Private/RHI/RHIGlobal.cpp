// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHICommandQueue.h"
#include "D3D12RHI/D3D12Graphics.h"

std::shared_ptr<NRHIGraphics> NRHIGlobal::DynamicRHI;
std::shared_ptr<NRHICommandQueue> NRHIGlobal::PrimaryCommandQueue;

void NRHIGlobal::InitDynamicRHI()
{
#if PLATFORM_WINDOWS
	DynamicRHI = ND3D12Graphics::GenerateGraphics();
#endif

	if (!DynamicRHI)
	{
		throw NotSupportedException();
	}

	DynamicRHI->Init();
	PrimaryCommandQueue = DynamicRHI->GetPrimaryQueue();
}

void NRHIGlobal::ShutdownDynamicRHI()
{
	PrimaryCommandQueue.reset();
	DynamicRHI.reset();
}