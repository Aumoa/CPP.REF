#pragma once

namespace Touhou
{
	class PlayableCharacterBase : public GameObject
	{
		String filename;

	protected:
		PlayableCharacterBase( String name, String filename );

		void LoadAnimatorController();
	};
}