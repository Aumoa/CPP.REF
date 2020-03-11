#pragma once

namespace Touhou
{
	class KeyboardMovement : public Component
	{
		double speed = 3.0;

	public:
		KeyboardMovement();

		object Clone() override;
		void Update( Time& time, Input& input ) override;
	};
}