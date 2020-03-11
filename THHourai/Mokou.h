#pragma once

namespace Touhou
{
	class Mokou : public PlayableCharacterBase
	{
		RefPtr<GameObject> ayaOriginal;

	public:
		Mokou( String name );

	private:
		void LoadAssets();
	};
}