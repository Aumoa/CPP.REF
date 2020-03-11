#pragma once

namespace Touhou
{
	class CombatBase : public Component
	{
		double maxHealthPoint = 0;
		double maxMagicPoint = 0;

		double currentHealthPoint = 0;
		double currentMagicPoint = 0;

	public:
		CombatBase();

		object Clone() override;

		vs_property_auto( double, MaxHealthPoint );
		vs_property_auto( double, MaxMagicPoint );
		vs_property_auto( double, CurrentHealthPoint );
		vs_property_auto( double, CurrentMagicPoint );
	};
}