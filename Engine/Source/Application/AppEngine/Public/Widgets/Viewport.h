// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Window.h"

interface IApplicationInterface;

class APPENGINE_API SViewport : public SWindow
{
	GENERATED_BODY(SViewport)

public:
	SViewport();

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) override;
};