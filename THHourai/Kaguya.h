#pragma once

namespace Touhou
{
	class Kaguya : public PlayableCharacterBase
	{
		RefPtr<GameObject> ayaOriginal;

	public:
		Kaguya( String name );

	private:
		void LoadAssets();
	};
}