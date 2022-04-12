// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Geometry.h"

namespace libty::inline SlateCore
{
	class SWidget;
}

namespace libty::inline SlateCore
{
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
}