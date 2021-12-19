// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIFactoryChild.h"
#include "RHIEnums.h"

interface IRHITextLayout : implements IRHIFactoryChild
{
	GENERATED_INTERFACE_BODY(IRHITextLayout)

	virtual void SetMaxSize(const Vector2& InSize) = 0;
	virtual Vector2 GetMaxSize() = 0;
	virtual Vector2 GetDesiredSize() = 0;
	virtual void SetTextAlignment(ERHITextAlignment Alignment) = 0;
	virtual ERHITextAlignment GetTextAlignment() = 0;
	virtual void SetParagraphAlignment(ERHIParagraphAlignment Alignment) = 0;
	virtual ERHIParagraphAlignment GetParagraphAlignment() = 0;
};