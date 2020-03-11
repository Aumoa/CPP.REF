#pragma once

namespace SC::Game::UI
{
	/// <summary> ���콺 ��ư�� ǥ���մϴ�. </summary>
	enum class MouseButtonType
	{
		/// <summary> ���� ��ư�� ��Ÿ���ϴ�. </summary>
		LeftButton = 0,

		/// <summary> ������ ��ư�� ��Ÿ���ϴ�. </summary>
		RightButton = 1,

		/// <summary> ��� ��ư�� ��Ÿ���ϴ�. </summary>
		MiddleButton = 2,

		/// <summary> ù ��° Ȯ�� ��ư�� ��Ÿ���ϴ�. </summary>
		X1Button = 3,
		
		/// <summary> �� ��° Ȯ�� ��ư�� ��Ÿ���ϴ�. </summary>
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