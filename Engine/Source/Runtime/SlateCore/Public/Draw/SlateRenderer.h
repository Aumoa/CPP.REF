// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext2D;
interface IRHISolidColorBrush;
class SlateWindowElementList;

class SLATECORE_API SSlateRenderer : implements SObject
{
	GENERATED_BODY(SSlateRenderer)

private:
	SPROPERTY(TintBrush)
	IRHISolidColorBrush* TintBrush = nullptr;

public:
	SSlateRenderer();

	void PopulateCommands(IRHIDeviceContext2D* CommandList, SlateWindowElementList& Elements);

private:
	IRHISolidColorBrush* GetTintBrush(IRHIDeviceContext2D* CommandList, const Color& TintColor, float RenderOpacity);
};