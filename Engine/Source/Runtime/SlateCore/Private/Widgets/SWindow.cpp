// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/SWindow.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"
#include "RHI/RHIViewport.h"

SWindow::SWindow()
{
}

SWindow::~SWindow() noexcept
{
}

void SWindow::AttachWindow(std::shared_ptr<NGenericWindow> InNativeWindow)
{
	check(!NativeWindow);
	check(!Viewport);

	auto* DynamicRHI = NRHIGlobal::GetDynamicRHI();
	NativeWindow = std::move(InNativeWindow);
	Viewport = DynamicRHI->CreateViewport(NRHIGlobal::GetPrimaryCommandQueue(), NativeWindow.get());
}