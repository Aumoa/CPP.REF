#pragma once

namespace SC::Game::UI
{
	/// <summary> 마우스 버튼을 표현합니다. </summary>
	enum class MouseButtonType
	{
		/// <summary> 왼쪽 버튼을 나타냅니다. </summary>
		LeftButton = 0,

		/// <summary> 오른쪽 버튼을 나타냅니다. </summary>
		RightButton = 1,

		/// <summary> 가운데 버튼을 나타냅니다. </summary>
		MiddleButton = 2,

		/// <summary> 첫 번째 확장 버튼을 나타냅니다. </summary>
		X1Button = 3,
		
		/// <summary> 두 번째 확장 버튼을 나타냅니다. </summary>
		X2Button = 4
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::UI::MouseButtonType value )
{
	static constexpr const wchar_t* context[] =
	{
		L"LeftButton",
		L"RightButton",
		L"MiddleButton",
		L"X1Button",
		L"X2Button",
	};

	if ( ( int )value >= 0 && ( int )value < 5 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Game.UI.MouseButtonType(" << ( int )value << L")";
	}
}