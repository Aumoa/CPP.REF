#pragma once

namespace SC
{
	/// <summary> 앱에서 관리되지 않는 네이티브 예외가 발생하였을 경우 발생하는 예외입니다. 이 클래스는 상속될 수 없습니다. </summary>
	class NativeException final : public Exception
	{
		std::exception_ptr native_ptr;

	public:
		/// <summary> <see cref="NativeException"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="native"> 발생한 네이티브 예외 포인터를 전달합니다. </param>
		NativeException( std::exception_ptr native );

		/// <summary> (Visual Studio 전용) 네이티브 예외에 대한 포인터를 가져옵니다. </summary>
		vs_property_get( std::exception_ptr, NativePtr );

		/// <summary> 네이티브 예외에 대한 포인터를 가져옵니다. </summary>
		[[nodiscard]] std::exception_ptr NativePtr_get() const;
	};
}