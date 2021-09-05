// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Widget.h"

class SLATECORE_API SLeafWidget : public SWidget
{
	GENERATED_BODY(SLeafWidget)

public:
	SLeafWidget(const std::wstring& name = GenerateAutoNumberedName());
	virtual ~SLeafWidget() override;

protected:
	virtual void OnArrangeChildren(ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const override;

private:
	static std::wstring GenerateAutoNumberedName();
};