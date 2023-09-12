// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Rendering/SlateWindowElementList.h"
#include "Widgets/SWindow.h"

NSlateWindowElementList::NSlateWindowElementList()
{
}

NSlateWindowElementList::NSlateWindowElementList(NSlateWindowElementList&& Rhs) noexcept
	: RenderTarget(std::move(Rhs.RenderTarget))
{
}

NSlateWindowElementList::~NSlateWindowElementList() noexcept
{
}

NSlateRenderElement& NSlateWindowElementList::Add()
{
	return UnorderedElements.emplace_back();
}

NSlateWindowElementList NSlateWindowElementList::Init(std::shared_ptr<SWindow> InRenderTargetWindow)
{
	NSlateWindowElementList Val;
	Val.RenderTarget = std::move(InRenderTargetWindow);
	return Val;
}