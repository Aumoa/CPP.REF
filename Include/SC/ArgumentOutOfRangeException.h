#pragma once

namespace SC
{
	/// <summary> 인수 값이 호출된 메서드에서 정의한 값의 허용 범위를 벗어날 때 발생하는 예외입니다. </summary>
	class ArgumentOutOfRangeException : public ArgumentException
	{
	protected:
		/// <summary> <see cref="ArgumentNullException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 예외 이름입니다. </param>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentOutOfRangeException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="ArgumentNullException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentOutOfRangeException( Exception* innerException = nullptr );

		/// <summary> <see cref="ArgumentNullException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		ArgumentOutOfRangeException( String message, Exception* innerException = nullptr );
	};
}