#pragma once

namespace SC::Game
{
	/// <summary> 게임 플랫폼에서 지원하는 디버그 관련 함수를 사용합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class Debug abstract final : virtual public SC::Object
	{
	public:
		/// <summary> 텍스트 로그를 발생시킵니다. </summary>
		/// <param name="logText"> 텍스트 변환 가능한 개체를 전달합니다. </param>
		static void Log( object logText );
	};
}