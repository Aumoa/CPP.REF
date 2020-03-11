#pragma once

namespace SC::Game::UI
{
	/// <summary> 텍스트 수직 정렬 방식을 표현합니다. </summary>
	enum class TextVerticalAlignment
	{
		/// <summary> 텍스트를 위쪽으로 정렬합니다. </summary>
		Top = 0,

		/// <summary> 텍스트를 가운데로 정렬합니다. </summary>
		Center = 1,

		/// <summary> 텍스트를 아래쪽으로 정렬합니다. </summary>
		Bottom = 2
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::UI::TextVerticalAlignment value )
{
	static constexpr const wchar_t* context[] =
	{
		L"Top",
		L"Center",
		L"Bottom",
	};

	if ( ( int )value >= 0 && ( int )value < 3 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Game.UI.TextVerticalAlignment(" << ( int )value << L")";
	}
}