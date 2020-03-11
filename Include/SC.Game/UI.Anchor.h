#pragma once

namespace SC::Game::UI
{
	/// <summary> UI ��ġ�� ����� �������� ǥ���մϴ�. </summary>
	enum class Anchor
	{
		/// <summary> ���� �� ��ġ�� ����մϴ�. </summary>
		LeftTop = 0,
		
		/// <summary> ���� ��� ��ġ�� ����մϴ�. </summary>
		Top = 1,

		/// <summary> ������ �� ��ġ�� ����մϴ�. </summary>
		RightTop = 2,

		/// <summary> ���� ��� ��ġ�� ����մϴ�. </summary>
		Left = 3,

		/// <summary> ��� ��ġ�� ����մϴ�. </summary>
		Center = 4,
		
		/// <summary> ������ ��� ��ġ�� ����մϴ�. </summary>
		Right = 5,

		/// <summary> ���� �Ʒ� ��ġ�� ����մϴ�. </summary>
		LeftBottom = 6,

		/// <summary> �Ʒ��� ��� ��ġ�� ����մϴ�. </summary>
		Bottom = 7,

		/// <summary> ������ �Ʒ� ��ġ�� ����մϴ�. </summary>
		RightBottom = 8
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::UI::Anchor value )
{
	static constexpr const wchar_t* context[] =
	{
		L"LeftTop",
		L"Top",
		L"RightTop",
		L"Left",
		L"Center",
		L"Right",
		L"LeftBottom",
		L"Bottom",
		L"RightBottom"
	};

	if ( ( int )value >= 0 && ( int )value <= 8 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Game.UI.Anchor(" << ( int )value << L")";
	}
}