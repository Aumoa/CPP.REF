// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Widget.h"

class SLATECORE_API SLeafWidget : public SWidget
{
	GENERATED_BODY(SLeafWidget)

public:
	SLeafWidget(const std::wstring& name);
	virtual ~SLeafWidget() override;

protected:
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const override;
};