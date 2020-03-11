#pragma once

namespace SC::Game
{
	/// <summary> 카메라의 배경 칠 모드를 단일 색상 모드로 설정합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class CameraClearModeColor final : public CameraClearMode
	{
		Drawing::Color clearColor;

	public:
		/// <summary> <see cref="CameraClearModeColor"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		CameraClearModeColor( Drawing::Color clearColor = Drawing::Color( 0, 0, 0, 0 ) )
			: clearColor( clearColor )
		{

		}

		/// <summary> (Visual Studio 전용) 설정된 단일 색상을 가져옵니다. </summary>
		vs_property_get( Drawing::Color, ClearColor );

		/// <summary> 설정된 단일 색상을 가져옵니다. </summary>
		Drawing::Color ClearColor_get()
		{
			return clearColor;
		}
	};
}