#pragma once

namespace SC
{
	/// <summary> 잘못된 캐스팅을 수행하였을 경우 발생하는 예외입니다. </summary>
	class InvalidCastException : public Exception
	{
	protected:
		/// <summary> <see cref="InvalidCastException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 예외 이름입니다. </param>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		InvalidCastException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="InvalidCastException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		InvalidCastException( Exception* innerException = nullptr );

		/// <summary> <see cref="InvalidCastException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		InvalidCastException( String message, Exception* innerException = nullptr );
	};
}