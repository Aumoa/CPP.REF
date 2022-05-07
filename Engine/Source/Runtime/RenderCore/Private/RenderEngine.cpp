// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RenderEngine.h"
#include "RenderContext.h"

using namespace ::libty;

SRenderEngine::SRenderEngine()
	: Super()
{
	IRHIFactory* Factory = IApplicationInterface::Get().GetFactory();
	IRHIAdapter* Adapter = Factory->GetAdapter(0);

	RHIDevice = Factory->CreateDevice(Adapter);
	RHIQueue = RHIDevice->CreateCommandQueue(ERHICommandListType::Direct);
}

SRenderContext* SRenderEngine::CreateRenderContext()
{
	auto* context = gcnew SRenderContext();
	context->OwningDevice = RHIDevice;
	context->RenderQueue = RHIQueue;
	context->CommandAllocator = RHIDevice->CreateCommandAllocator(ERHICommandListType::Direct);
	context->CommandList = RHIDevice->CreateCommandList(context->CommandAllocator, ERHICommandListType::Direct, nullptr);
	return context;
}