#pragma once

/* FOR DEBUG COMPONENT */

namespace Touhou
{
	class AutoRotate : public Component
	{
		double v = 0;

	public:
		AutoRotate()
		{

		}

		object Clone() override
		{
			return new AutoRotate();
		}

		void Update( Time& time, Input& input ) override
		{
			Transform->Rotation = Quaternion::AngleAxis( v, Vector3::Right );
			v += time.DeltaTimeInSeconds;
		}
	};
}