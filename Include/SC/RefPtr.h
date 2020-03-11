#pragma once

namespace SC
{
	/// <summary> SC ��ü�� ���� Ƚ���� �����ϴ� ����Ʈ ������ Ŭ�����Դϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
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

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( std::nullptr_t );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( T* ptr = nullptr );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( RefPtr& ptr );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( const RefPtr& ptr );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		template< class O, typename std::enable_if_t<where<T, O>>* = nullptr >
		inline RefPtr( RefPtr<O>& ptr );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( RefPtr&& ptr ) noexcept;

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		template< class O, typename std::enable_if_t<where<T, O>>* = nullptr >
		inline RefPtr( RefPtr<O>&& ptr );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="value"> ���� �����ڰ� �����ϴ� �� ���� ��ü�� �����մϴ�. </summary>
		template< class O, typename std::enable_if_t<where<T, O>>* = nullptr >
		inline RefPtr( const O& value );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		template< class O, typename std::enable_if_t<!where<Object, O> && !std::is_pointer_v<O> && std::is_same_v<Object, T>>* = nullptr >
		inline RefPtr( const O& native_value );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( const char* str );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( const wchar_t* str );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( const std::string& str );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( const std::string_view& str );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( const std::wstring& str );

		/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline RefPtr( const std::wstring_view& str );

		/// <summary> ���� ��ü�� ���� �����մϴ�. </summary>
		/// <param name="ptr"> ���� ������ ������ ������ �����մϴ�. </param>
		inline void Reset( T* ptr = nullptr );

		/// <summary> ��ü �����͸� �����ɴϴ�. </summary>
		[[nodiscard]] T* Get() const;

		/// <summary> ��ü �������� �ּҸ� �����ɴϴ�. </summary>
		[[nodiscard]] T** GetAddressOf();

		/// <summary> ��ü �������� �ּҸ� �����ɴϴ�. ���� ��ü�� ������ ��� ���� ��ü�� Release �˴ϴ�. </summary>
		[[nodiscard]] T** ReleaseAndGetAddressOf();

		/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. ������ ��� false�� ��ȯ�˴ϴ�. </summary>
		/// <param name="outptr"> ��ȯ�� �Ϸ�� ��� ��ü�� ����� ���� �������� �ּҸ� �����մϴ�. </param>
		template< class O, std::enable_if_t<where<Object, O> && !where<ValueType, O>>* = nullptr >
		[[nodiscard]] inline bool Is( O** outptr = nullptr );

		/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. ������ ��� false�� ��ȯ�˴ϴ�. </summary>
		/// <param name="outptr"> ��ȯ�� �Ϸ�� ��� ��ü�� ����� ���� �������� �ּҸ� �����մϴ�. </param>
		template< class O, std::enable_if_t<where<ValueType, O>>* = nullptr >
		[[nodiscard]] inline bool Is( O* outptr = nullptr );

		/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. ������ ��� false�� ��ȯ�˴ϴ�. </summary>
		/// <param name="outptr"> ��ȯ�� �Ϸ�� ��� ��ü�� ����� ���� �������� �ּҸ� �����մϴ�. </param>
		template< class O, std::enable_if_t<!where<Object, O> && !std::is_pointer_v<O>>* = nullptr >
		[[nodiscard]] inline bool Is( O* outptr = nullptr );

		/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. </summary>
		/// <exception cref="InvalidCastException"> ��ü�� �� ��ȯ�� �ùٸ��� ���� ��� �߻��մϴ�. </exception>
		/// <exception cref="NullReferenceException"> ���� �����Ͱ� null�� ����ų ��� �߻��մϴ�. </exception>
		template< class O, std::enable_if_t<where<Object, O> && !where<ValueType, O>>* = nullptr >
		[[nodiscard]] inline RefPtr<O> As();

		/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. </summary>
		/// <exception cref="InvalidCastException"> ��ü�� �� ��ȯ�� �ùٸ��� ���� ��� �߻��մϴ�. </exception>
		/// <exception cref="NullReferenceException"> ���� �����Ͱ� null�� ����ų ��� �߻��մϴ�. </exception>
		template< class O, std::enable_if_t<is_refptr_v<O>> * = nullptr >
		[[nodiscard]] inline O As();

		/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. </summary>
		/// <exception cref="InvalidCastException"> ��ü�� �� ��ȯ�� �ùٸ��� ���� ��� �߻��մϴ�. </exception>
		/// <exception cref="NullReferenceException"> ���� �����Ͱ� null�� ����ų ��� �߻��մϴ�. </exception>
		template< class O, std::enable_if_t<where<ValueType, O>> * = nullptr >
		[[nodiscard]] inline O As();

		/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. </summary>
		/// <exception cref="InvalidCastException"> ��ü�� �� ��ȯ�� �ùٸ��� ���� ��� �߻��մϴ�. </exception>
		/// <exception cref="NullReferenceException"> ���� �����Ͱ� null�� ����ų ��� �߻��մϴ�. </exception>
		template< class O, std::enable_if_t<!where<Object, O> && !is_refptr_v<O>> * = nullptr >
		[[nodiscard]] inline O As();

		/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. ĳ���ÿ� ������ ��� ���ܸ� �߻���Ű�� �ʰ� nullptr�� ��ȯ�մϴ�. </summary>
		template< class O >
		[[nodiscard]] inline auto TryAs();

		/// <summary> (Visual Studio ����) ��ü �����Ͱ� �ùٸ� �������� �ĺ��ϴ� ���� �����ɴϴ�. </summary>
		vs_property_get( bool, IsValid );

		/// <summary> ��ü �����Ͱ� �ùٸ� �������� �ĺ��ϴ� ���� �����ɴϴ�. </summary>
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