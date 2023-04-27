// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Geometry.h"
//#include <memory>

class SWidget;

class GAME_API ArrangedWidget
{
	std::shared_ptr<SWidget> Widget;
	Geometry MyGeometry;

public:
	ArrangedWidget(std::shared_ptr<SWidget> InWidget, const Geometry& InGeometry);

	String ToString() const;

	const std::shared_ptr<SWidget>& GetWidget() const { return Widget; }
	const Geometry& GetGeometry() const { return MyGeometry; }

	bool operator ==(const ArrangedWidget& Rhs) const;
	bool operator !=(const ArrangedWidget& Rhs) const;
};