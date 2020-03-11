#pragma once

namespace Touhou
{
	class Scene0_Terrain : public GameObject
	{
	public:
		Scene0_Terrain( String name );

	private:
		void InitializeComponents();
	};
}