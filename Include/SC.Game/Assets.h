#pragma once

namespace SC::Game
{
	/// <summary> 게임 데이터 자산을 표현합니다. </summary>
	class Assets : virtual public Object
	{
		String name;

	protected:
		/// <summary> <see cref="Assets"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 자산 데이터의 식별자 이름을 제공합니다. </param>
		Assets( String name );

	public:
		/// <summary> (Visual Studio 전용) 자산 데이터의 식별자 이름을 가져옵니다. </summary>
		vs_property( String, Name );

		/// <summary> 자산 데이터의 식별자 이름을 가져옵니다. </summary>
		String Name_get();
	};
}