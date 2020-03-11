#pragma once

namespace SC
{
	/// <summary> Null 참조 포인터 개체에 역참조하려고 할 경우 발생하는 예외입니다. </summary>
	class NullReferenceException : public Exception
	{
	protected:
		/// <summary> <see cref="NullReferenceException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 예외 이름입니다. </param>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		NullReferenceException( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="NullReferenceException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		NullReferenceException( Exception* innerException = nullptr );

		/// <summary> <see cref="NullReferenceException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		NullReferenceException( String message, Exception* innerException = nullptr );
	};
}