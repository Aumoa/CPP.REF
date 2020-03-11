#pragma once

namespace Touhou
{
	class Movement : public Component
	{
		double speed = 1;
		Vector3 moveTo;
		RefPtr<Animator> animator;

	public:
		Movement();

		object Clone() override;
		void Start() override;
		void FixedUpdate( Time& fixedTime ) override;

		vs_property( double, Velocity );
		double Velocity_get();
		void Velocity_set( double value );
		vs_property_set( Vector3, MoveTo );
		void MoveTo_set( Vector3 value );
	};
}