#pragma once

namespace SC
{
	/// <summary> 앱 실행 중 발생하는 모든 예외에 대한 기본 항목을 제공합니다. </summary>
	class Exception : virtual public Object
	{
		String name;
		String message;
		RefPtr<Exception> innerException = nullptr;

	protected:
		/// <summary> <see cref="Exception"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		/// <param name="name"> 예외 이름입니다. </param>
		Exception( String message, Exception* innerException, String name );

	public:
		/// <summary> <see cref="Exception"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Exception();

		/// <summary> <see cref="Exception"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="message"> 예외에 대한 이유를 설명하는 오류 메시지입니다. </param>
		/// <param name="innerException"> 현재 예외를 발생시킨 예외입니다. </param>
		Exception( String message, Exception* innerException = nullptr );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 예외에 대한 요약 정보의 텍스트 표현을 가져옵니다. </summary>
		String ToString() override;

		/// <summary> (Visual Studio 전용) 현재 예외 이름을 가져옵니다. </summary>
		vs_property_get( String, Name );

		/// <summary> 현재 예외 이름을 가져옵니다. </summary>
		String Name_get();

		/// <summary> (Visual Studio 전용) 현재 예외를 설명하는 메시지를 가져옵니다. </summary>
		vs_property_get( String, Message );

		/// <summary> 현재 예외를 설명하는 메시지를 가져옵니다. </summary>
		String Message_get();

		/// <summary> (Visual Studio 전용) 현재 예외를 발생시킨 <see cref="SC::Exception"/> 개체를 가져옵니다. </summary>
		vs_property_get( RefPtr<Exception>, InnerException );

		/// <summary> 현재 예외를 발생시킨 <see cref="Exception"/> 개체를 가져옵니다. </summary>
		RefPtr<Exception> InnerException_get();
	};
}