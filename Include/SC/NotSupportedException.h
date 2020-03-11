#pragma once

namespace SC
{
	/// <summary> 호출한 기능이 지원되지 않는 개체에 대해 작업을 수행하려고 했을 경우 발생하는 예외입니다. </summary>
	class NotSupportedException : public Exception
	{
	protected:
		/// <summary> <see cref="NotSupportedException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		/// <param name="name"> 예외 이름입니다. </param>
		NotSupportedException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="NotSupportedException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		NotSupportedException( Exception* innerException = nullptr );

		/// <summary> <see cref="NotSupportedException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		NotSupportedException( String message, Exception* innerException = nullptr );
	};
}