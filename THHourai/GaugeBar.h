#pragma once

namespace Touhou
{
	class GaugeBar : public UI::FixedPanel
	{
		double maxValue = 0;
		double currentValue = 0;

	public:
		GaugeBar();

		vs_property_auto( double, MaxValue );
		vs_property_auto( double, CurrentValue );
	};
}