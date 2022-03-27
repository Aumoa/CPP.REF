// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RenderEngine.h"
#include "IApplicationInterface.h"
#include "RenderContext.h"

GENERATE_BODY(SRenderEngine);

SRenderEngine::SRenderEngine()
	: Super()
{
	IRHIFactory* factory = IApplicationInterface::Get().GetFactory();
	IRHIAdapter* adapter = factory->GetAdapter(0);

	_device = factory->CreateDevice(adapter);
	_commandQueue = _device->CreateCommandQueue(ERHICommandListType::Direct);
}

SRenderContext* SRenderEngine::CreateRenderContext()
{
	auto* context = gcnew SRenderContext();
	context->OwningDevice = _device;
	context->RenderQueue = _commandQueue;
	context->CommandAllocator = _device->CreateCommandAllocator(ERHICommandListType::Direct);
	context->CommandList = _device->CreateCommandList(context->CommandAllocator, ERHICommandListType::Direct, nullptr);
	return context;
}