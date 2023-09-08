// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Geometry.h"

class SWidget;

class SLATECORE_API NArrangedWidget
{
	std::shared_ptr<SWidget> Widget = nullptr;
	NGeometry MyGeometry;

public:
	NArrangedWidget(SWidget* InWidget, const NGeometry& InGeometry);

	String ToString() const;

	SWidget* GetWidget() const { return Widget.get(); }
	const NGeometry& GetGeometry() const { return MyGeometry; }

	bool operator ==(const NArrangedWidget& Rhs) const;
	bool operator !=(const NArrangedWidget& Rhs) const;
};