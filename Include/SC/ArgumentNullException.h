#pragma once

namespace SC
{
	/// <summary> Null 포인터를 올바르지 않은 인수로 받는 함수에 Null 포인터가 전달되면 발생하는 예외입니다. </summary>
	class ArgumentNullException : public ArgumentException
	{
	protected:
		/// <summary> <see cref="ArgumentNullException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 예외 이름입니다. </param>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentNullException( String message, Exception* innerException, String name );

		/// <summary> <see cref="ArgumentNullException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 예외 이름입니다. </param>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="parameterName"> 현재 예외를 발생시킨 매개 변수의 이름입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentNullException( String message, String parameterName, Exception* innerException, String name );

	public:
		/// <summary> <see cref="ArgumentNullException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentNullException( Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentNullException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentNullException( String message, Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentNullException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="parameterName"> 현재 예외를 발생시킨 매개 변수의 이름입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentNullException( String message, String parameterName, Exception* innerException = nullptr );
	};
}