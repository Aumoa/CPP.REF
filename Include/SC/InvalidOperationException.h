#pragma once

namespace SC
{
	/// <summary> 메서드 호출이 개체의 현재 상태에 대해 유효하지 않을 때 발생하는 예외입니다. </summary>
	class InvalidOperationException : public Exception
	{
	protected:
		/// <summary> <see cref="InvalidOperationException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 예외 이름입니다. </param>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		InvalidOperationException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="InvalidOperationException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		InvalidOperationException( Exception* innerException = nullptr );

		/// <summary> <see cref="InvalidOperationException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		InvalidOperationException( String message, Exception* innerException = nullptr );
	};
}