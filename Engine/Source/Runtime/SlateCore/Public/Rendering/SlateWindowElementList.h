// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/SlateRenderElement.h"

class SWindow;

struct SLATECORE_API NSlateWindowElementList
{
	NSlateWindowElementList(const NSlateWindowElementList&) = delete;

	std::shared_ptr<SWindow> RenderTarget;
	std::vector<NSlateRenderElement> UnorderedElements;

	NSlateWindowElementList();
	NSlateWindowElementList(NSlateWindowElementList&& Rhs) noexcept;
	~NSlateWindowElementList() noexcept;

	NSlateRenderElement& Add();

	static NSlateWindowElementList Init(std::shared_ptr<SWindow> InRenderTargetWindow);
};