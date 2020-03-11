#pragma once

namespace Touhou
{
	class GameVar : virtual public Object
	{
	public:
		static int pickedCharacterType;
		static RefPtr<GameObject> playableCharacter;
	};
}