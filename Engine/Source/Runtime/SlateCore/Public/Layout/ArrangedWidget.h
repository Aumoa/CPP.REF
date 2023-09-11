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
	NArrangedWidget(std::shared_ptr<SWidget> InWidget, const NGeometry& InGeometry);

	String ToString() const;

	std::shared_ptr<SWidget> GetWidget() const { return Widget; }
	const NGeometry& GetGeometry() const { return MyGeometry; }

	bool operator ==(const NArrangedWidget& Rhs) const { return Widget != Rhs.Widget; }
	auto operator <=>(const NArrangedWidget& Rhs) const { return Widget <=> Rhs.Widget; }
};