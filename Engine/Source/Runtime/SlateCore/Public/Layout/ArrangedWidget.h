// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Geometry.h"

class SWidget;

class SLATECORE_API ArrangedWidget
{
	const SWidget* _widget = nullptr;
	Geometry _geometry;

public:
	ArrangedWidget(const SWidget* widget, const Geometry& geometry);

	std::wstring ToString(std::wstring_view formatArgs = L"") const;

	const SWidget* GetWidget() const { return _widget; }
	const Geometry& GetGeometry() const { return _geometry; }

	bool operator ==(const ArrangedWidget& rhs) const;
	bool operator !=(const ArrangedWidget& rhs) const;
};