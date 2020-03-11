#pragma once

namespace Touhou
{
	class MouseEventDispatcher : public Component
	{
		RefPtr<Movement> movement;

	public:
		MouseEventDispatcher();

		object Clone() override;
		void Start() override;
		void Update( Time& time, Input& input ) override;
	};
}