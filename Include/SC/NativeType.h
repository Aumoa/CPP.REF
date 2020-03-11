#pragma once

namespace SC
{
	/// <summary> SC 클래스 형식에 포함되지 않은 네이티브 클래스 형식에 대한 래퍼입니다. 이 클래스는 상속될 수 없습니다. </summary>
	template< class T >
	class NativeType final : public ValueType
	{
		T* ptr = nullptr;

	public:
		/// <summary> <see cref="NativeType"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="value"> 지정된 형식의 네이티브 값을 전달합니다. </param>
		template< typename std::enable_if_t<!where<Object, T>> * = nullptr >
		inline NativeType( const T& value );
		inline ~NativeType() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		inline String ToString() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		inline bool Equals( object obj ) override;

		/// <summary> 네이티브 값 형식을 언박싱합니다. </summary>
		inline T Unboxing();

	private:
		template< class O, std::enable_if_t<Details::HasOperator<O>::value>* = nullptr >
		inline String ToStringT()
		{
			std::wstringstream wss;
			wss << *ptr;
			return wss.str().c_str();
		}

		template< class O, std::enable_if_t<!Details::HasOperator<O>::value>* = nullptr >
		inline String ToStringT()
		{
			return String::Format( "SC.NativeType<{0}>", new String( typeid( O ).name() ) );
		}
	};
}