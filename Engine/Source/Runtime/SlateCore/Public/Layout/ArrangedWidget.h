// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Geometry.h"

class SWidget;

class SLATECORE_API ArrangedWidget
{
	SWidget* Widget = nullptr;
	Geometry MyGeometry;

public:
	ArrangedWidget(SWidget* InWidget, const Geometry& InGeometry);

	std::wstring ToString(std::wstring_view InFormatArgs = L"") const;

	SWidget* GetWidget() const { return Widget; }
	const Geometry& GetGeometry() const { return MyGeometry; }

	bool operator ==(const ArrangedWidget& Rhs) const;
	bool operator !=(const ArrangedWidget& Rhs) const;
};