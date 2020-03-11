#pragma once

namespace Touhou
{
	class Bee : public GameObject
	{
	public:
		Bee( String name );

	private:
		void LoadAssets();
		void LoadAnimatorController();
	};
}