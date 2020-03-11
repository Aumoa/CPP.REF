#pragma once

namespace SC::Game
{
	/// <summary> 카메라의 배경 칠 모드에 대한 기본값을 제공합니다. </summary>
	class CameraClearMode abstract : virtual public Object
	{
		friend class CameraClearModeColor;
		friend class CameraClearModeSkybox;

	private:
		CameraClearMode()
		{

		}

	public:
	};
}