#pragma once

namespace SC::Game
{
	/// <summary> ���� ť�� �����Ǵ� ���̾� �׸��� ǥ���մϴ�. </summary>
	enum class RenderQueueLayer
	{
		Default = 0,
		Additive = 1,
		Transparent = 2
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::RenderQueueLayer value )
{
	static constexpr const wchar_t* context[] =
	{
		L"Default",
		L"Additive",
		L"Transparent",
	};

	if ( ( int )value >= 0 && ( int )value <= 2 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Game.RenderQueueLayer(" << ( int )value << L")";
	}
}