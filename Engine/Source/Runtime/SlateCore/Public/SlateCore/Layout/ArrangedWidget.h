// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "GC/WeakPtr.h"
#include "Geometry.h"

class SWidget;

class SLATECORE_API ArrangedWidget
{
	WeakPtr<SWidget> Widget = nullptr;
	Geometry MyGeometry;

public:
	ArrangedWidget(SWidget* InWidget, const Geometry& InGeometry);

	std::wstring ToString() const;

	SWidget* GetWidget() const { return Widget.Get(); }
	const Geometry& GetGeometry() const { return MyGeometry; }

	bool operator ==(const ArrangedWidget& Rhs) const;
	bool operator !=(const ArrangedWidget& Rhs) const;
};