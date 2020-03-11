#pragma once

namespace SC
{
	/// <summary> 요청한 메서드 또는 연산이 구현되지 않을 때 발생하는 예외입니다. </summary>
	class NotImplementedException : public Exception
	{
	protected:
		/// <summary> <see cref="NotImplementedException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		/// <param name="name"> 예외 이름입니다. </param>
		NotImplementedException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="NotImplementedException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		NotImplementedException( Exception* innerException = nullptr );

		/// <summary> <see cref="NotImplementedException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		NotImplementedException( String message, Exception* innerException = nullptr );
	};
}