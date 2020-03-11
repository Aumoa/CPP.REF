#pragma once

namespace SC::Game
{
	/// <summary> 앱 개체가 한 개를 초과하여 생성되었을 경우 발생하는 예외입니다. </summary>
	class AppDuplicateException : public Exception
	{
	public:
		/// <summary> <see cref="AppDuplicateException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		AppDuplicateException( Exception* innerException = nullptr );
	};
}