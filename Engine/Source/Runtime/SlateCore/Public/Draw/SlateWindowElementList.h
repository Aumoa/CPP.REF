// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Draw/SlateDrawElement.h"

class SWindow;

class SLATECORE_API SlateWindowElementList
{
	std::vector<SlateDrawElement> Elements;

public:
	SWindow* const PaintWindow;

public:
	SlateWindowElementList(SWindow* InPaintWindow);

	void SortByLayer();
	SlateDrawElement& AddUninitialized();
	void Clear();
	const std::vector<SlateDrawElement>& GetElements() const;
};