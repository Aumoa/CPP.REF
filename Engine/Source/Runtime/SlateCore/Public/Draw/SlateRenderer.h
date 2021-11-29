// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext2D;
interface IRHISolidColorBrush;
class SlateWindowElementList;

class SLATECORE_API SlateRenderer
{
private:
	IRHIDeviceContext2D* CommandList;
	std::shared_ptr<IRHISolidColorBrush> TintBrush;

public:
	SlateRenderer(IRHIDeviceContext2D* CommandList);
	~SlateRenderer();

	void PopulateCommands(SlateWindowElementList& Elements);

private:
	IRHISolidColorBrush* GetTintBrush(const Color& TintColor, float RenderOpacity);
};