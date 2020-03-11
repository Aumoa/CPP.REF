#pragma once

namespace SC
{
	/// <summary> 함수에 제공된 인수 중 하나가 유효하지 않을 때 발생하는 예외입니다. </summary>
	class ArgumentException : public Exception
	{
		String parameterName;

	protected:
		/// <summary> <see cref="ArgumentException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 예외 이름입니다. </param>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentException( String message, Exception* innerException, String name );

		/// <summary> <see cref="ArgumentException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 예외 이름입니다. </param>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="parameterName"> 현재 예외를 발생시킨 매개 변수의 이름입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentException( String message, String parameterName, Exception* innerException, String name );

	public:
		/// <summary> <see cref="ArgumentException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentException( Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentException( String message, Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="parameterName"> 현재 예외를 발생시킨 매개 변수의 이름입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentException( String message, String parameterName, Exception* innerException = nullptr );

		/// <summary> (Visual Studio 전용) 이 예외를 발생시킨 매개 변수의 이름을 가져옵니다. </summary>
		vs_property_get( String, ParamName );

		/// <summary> 이 예외를 발생시킨 매개 변수의 이름을 가져옵니다. </summary>
		String ParamName_get();
	};
}