#pragma once

namespace SC
{
	/// <summary> SC 개체를 참조 횟수로 관리하는 스마트 포인터 클래스입니다. 이 클래스는 상속될 수 없습니다. </summary>
	template< class T >
	class RefPtr final
	{
		template< class O >
		friend class RefPtr;
		friend struct WeakPtr;

	public:
		using _Myt = T;

	private:
		T* ptr = nullptr;

	private:
		inline size_t AddRef();
		inline size_t Release();

	public:
		inline ~RefPtr();

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( std::nullptr_t );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( T* ptr = nullptr );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( RefPtr& ptr );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( const RefPtr& ptr );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		template< class O, typename std::enable_if_t<where<T, O>>* = nullptr >
		inline RefPtr( RefPtr<O>& ptr );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( RefPtr&& ptr ) noexcept;

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		template< class O, typename std::enable_if_t<where<T, O>>* = nullptr >
		inline RefPtr( RefPtr<O>&& ptr );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="value"> 복사 생성자가 존재하는 값 형식 개체를 전달합니다. </summary>
		template< class O, typename std::enable_if_t<where<T, O>>* = nullptr >
		inline RefPtr( const O& value );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		template< class O, typename std::enable_if_t<!where<Object, O> && !std::is_pointer_v<O> && std::is_same_v<Object, T>>* = nullptr >
		inline RefPtr( const O& native_value );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( const char* str );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( const wchar_t* str );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( const std::string& str );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( const std::string_view& str );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( const std::wstring& str );

		/// <summary> <see cref="RefPtr"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline RefPtr( const std::wstring_view& str );

		/// <summary> 참조 개체를 새로 설정합니다. </summary>
		/// <param name="ptr"> 새로 참조할 포인터 형식을 전달합니다. </param>
		inline void Reset( T* ptr = nullptr );

		/// <summary> 개체 포인터를 가져옵니다. </summary>
		[[nodiscard]] T* Get() const;

		/// <summary> 개체 포인터의 주소를 가져옵니다. </summary>
		[[nodiscard]] T** GetAddressOf();

		/// <summary> 개체 포인터의 주소를 가져옵니다. 참조 개체가 존재할 경우 기존 개체는 Release 됩니다. </summary>
		[[nodiscard]] T** ReleaseAndGetAddressOf();

		/// <summary> 개체 포인터를 지정한 형식으로 캐스팅합니다. 실패할 경우 false가 반환됩니다. </summary>
		/// <param name="outptr"> 변환이 완료될 경우 개체가 저장될 참조 포인터의 주소를 전달합니다. </param>
		template< class O, std::enable_if_t<where<Object, O> && !where<ValueType, O>>* = nullptr >
		[[nodiscard]] inline bool Is( O** outptr = nullptr );

		/// <summary> 개체 포인터를 지정한 형식으로 캐스팅합니다. 실패할 경우 false가 반환됩니다. </summary>
		/// <param name="outptr"> 변환이 완료될 경우 개체가 저장될 참조 포인터의 주소를 전달합니다. </param>
		template< class O, std::enable_if_t<where<ValueType, O>>* = nullptr >
		[[nodiscard]] inline bool Is( O* outptr = nullptr );

		/// <summary> 개체 포인터를 지정한 형식으로 캐스팅합니다. 실패할 경우 false가 반환됩니다. </summary>
		/// <param name="outptr"> 변환이 완료될 경우 개체가 저장될 참조 포인터의 주소를 전달합니다. </param>
		template< class O, std::enable_if_t<!where<Object, O> && !std::is_pointer_v<O>>* = nullptr >
		[[nodiscard]] inline bool Is( O* outptr = nullptr );

		/// <summary> 개체 포인터를 지정한 형식으로 캐스팅합니다. </summary>
		/// <exception cref="InvalidCastException"> 개체의 형 변환이 올바르지 않을 경우 발생합니다. </exception>
		/// <exception cref="NullReferenceException"> 참조 포인터가 null을 가리킬 경우 발생합니다. </exception>
		template< class O, std::enable_if_t<where<Object, O> && !where<ValueType, O>>* = nullptr >
		[[nodiscard]] inline RefPtr<O> As();

		/// <summary> 개체 포인터를 지정한 형식으로 캐스팅합니다. </summary>
		/// <exception cref="InvalidCastException"> 개체의 형 변환이 올바르지 않을 경우 발생합니다. </exception>
		/// <exception cref="NullReferenceException"> 참조 포인터가 null을 가리킬 경우 발생합니다. </exception>
		template< class O, std::enable_if_t<is_refptr_v<O>> * = nullptr >
		[[nodiscard]] inline O As();

		/// <summary> 개체 포인터를 지정한 형식으로 캐스팅합니다. </summary>
		/// <exception cref="InvalidCastException"> 개체의 형 변환이 올바르지 않을 경우 발생합니다. </exception>
		/// <exception cref="NullReferenceException"> 참조 포인터가 null을 가리킬 경우 발생합니다. </exception>
		template< class O, std::enable_if_t<where<ValueType, O>> * = nullptr >
		[[nodiscard]] inline O As();

		/// <summary> 개체 포인터를 지정한 형식으로 캐스팅합니다. </summary>
		/// <exception cref="InvalidCastException"> 개체의 형 변환이 올바르지 않을 경우 발생합니다. </exception>
		/// <exception cref="NullReferenceException"> 참조 포인터가 null을 가리킬 경우 발생합니다. </exception>
		template< class O, std::enable_if_t<!where<Object, O> && !is_refptr_v<O>> * = nullptr >
		[[nodiscard]] inline O As();

		/// <summary> 개체 포인터를 지정한 형식으로 캐스팅합니다. 캐스팅에 실패한 경우 예외를 발생시키지 않고 nullptr을 반환합니다. </summary>
		template< class O >
		[[nodiscard]] inline auto TryAs();

		/// <summary> (Visual Studio 전용) 개체 포인터가 올바른 참조인지 식별하는 값을 가져옵니다. </summary>
		vs_property_get( bool, IsValid );

		/// <summary> 개체 포인터가 올바른 참조인지 식별하는 값을 가져옵니다. </summary>
		[[nodiscard]] inline bool IsValid_get() const;

		[[nodiscard]] inline T* operator->();
		[[nodiscard]] inline operator bool() const;
		inline RefPtr& operator=( RefPtr& other );
		inline RefPtr& operator=( const RefPtr& other );
		inline RefPtr& operator=( RefPtr&& other ) noexcept;
		[[nodiscard]] inline T** operator&();

		template< class O > [[nodiscard]] inline bool operator==( O right ) const;
		template< class O > [[nodiscard]] inline bool operator!=( O right ) const;
		template< class O > [[nodiscard]] inline bool operator< ( O right ) const;
		template< class O > [[nodiscard]] inline bool operator<=( O right ) const;
		template< class O > [[nodiscard]] inline bool operator> ( O right ) const;
		template< class O > [[nodiscard]] inline bool operator>=( O right ) const;
	};
}

#if !defined( __SC_NO_MACROS__ )
#define var RefPtr
#endif