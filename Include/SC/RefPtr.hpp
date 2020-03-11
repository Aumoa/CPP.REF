#pragma once

namespace SC
{
	template< class T >
	inline RefPtr<T>::~RefPtr()
	{
		Release();
	}

	template< class T >
	inline size_t RefPtr<T>::AddRef()
	{
		if ( ptr )
		{
			if ( !ptr->counter )
			{
				ptr->counter = new Details::ReferenceCounter();
			}

			return ptr->Object::AddRef();
		}
		else
		{
			return 0;
		}
	}
	
	template< class T >
	inline size_t RefPtr<T>::Release()
	{
		if ( ptr )
		{
			auto ret = ptr->Object::Release();
			ptr = nullptr;
			return ret;
		}
		else
		{
			return 0;
		}
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( std::nullptr_t )
	{

	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( T* ptr )
		: ptr( ptr )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( RefPtr& ptr )
		: ptr( ptr.Get() )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( const RefPtr& ptr )
		: RefPtr( ( RefPtr& )ptr )
	{

	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	template< class O, typename std::enable_if_t<where<T, O>>* arg1 >
	inline RefPtr<T>::RefPtr( RefPtr<O>& ptr )
		: ptr( ptr.Get() )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( RefPtr&& ptr ) noexcept
		: ptr( ptr.ptr )
	{
		ptr.ptr = nullptr;
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	template< class O, typename std::enable_if_t<where<T, O>>* arg1 >
	inline RefPtr<T>::RefPtr( RefPtr<O>&& ptr )
		: ptr( ptr.ptr )
	{
		ptr.ptr = nullptr;
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	template< class O, typename std::enable_if_t<where<T, O>>* arg1 >
	inline RefPtr<T>::RefPtr( const O& value )
		: ptr( new O( value ) )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	template< class O, typename std::enable_if_t<!where<Object, O> && !std::is_pointer_v<O> && std::is_same_v<Object, T>>* arg1 >
	inline RefPtr<T>::RefPtr( const O& native_value )
		: ptr( new NativeType<O>( native_value ) )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( const char* str )
		: ptr( new String( str ) )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( const wchar_t* str )
		: ptr( new String( str ) )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( const std::string& str )
		: ptr( new String( str.c_str() ) )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( const std::string_view& str )
		: ptr( new String( str.data() ) )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( const std::wstring& str )
		: ptr( new String( str.c_str() ) )
	{
		AddRef();
	}

	/// <summary> <see cref="RefPtr"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline RefPtr<T>::RefPtr( const std::wstring_view& str )
		: ptr( new String( str.data() ) )
	{
		AddRef();
	}

	/// <summary> ���� ��ü�� ���� �����մϴ�. </summary>
	/// <param name="ptr"> ���� ������ ������ ������ �����մϴ�. </param>
	template< class T >
	inline void RefPtr<T>::Reset( T* ptr )
	{
		Release();
		this->ptr = ptr;
		AddRef();
	}

	/// <summary> ��ü �����͸� �����ɴϴ�. </summary>
	template< class T >
	[[nodiscard]] inline T* RefPtr<T>::Get() const
	{
		return ( T* )ptr;
	}

	/// <summary> ��ü �������� �ּҸ� �����ɴϴ�. </summary>
	template< class T >
	[[nodiscard]] inline auto RefPtr<T>::GetAddressOf() -> T**
	{
		return &ptr;
	}

	/// <summary> ��ü �������� �ּҸ� �����ɴϴ�. ���� ��ü�� ������ ��� ���� ��ü�� Release �˴ϴ�. </summary>
	template< class T >
	[[nodiscard]] inline auto RefPtr<T>::ReleaseAndGetAddressOf() -> T**
	{
		Release();
		return &ptr;
	}

	/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. ������ ��� false�� ��ȯ�˴ϴ�. </summary>
	/// <param name="outptr"> ��ȯ�� �Ϸ�� ��� ��ü�� ����� ���� �������� �ּҸ� �����մϴ�. </param>
	template< class T >
	template< class O, std::enable_if_t<where<Object, O> && !where<ValueType, O>>* >
	[[nodiscard]] inline bool RefPtr<T>::Is( O** outptr )
	{
		if ( auto ptr = dynamic_cast< O* >( this->ptr ); ptr )
		{
			if ( outptr )
			{
				*outptr = ptr;
				ptr->Object::AddRef();
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. ������ ��� false�� ��ȯ�˴ϴ�. </summary>
	/// <param name="outptr"> ��ȯ�� �Ϸ�� ��� ��ü�� ����� ���� �������� �ּҸ� �����մϴ�. </param>
	template< class T >
	template< class O, std::enable_if_t<where<ValueType, O>>* >
	[[nodiscard]] inline bool RefPtr<T>::Is( O* outptr )
	{
		if ( auto ptr = dynamic_cast< O* >( this->ptr ); ptr )
		{
			if ( outptr )
			{
				// COPY
				*outptr = *ptr;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. ������ ��� false�� ��ȯ�˴ϴ�. </summary>
	/// <param name="outptr"> ��ȯ�� �Ϸ�� ��� ��ü�� ����� ���� �������� �ּҸ� �����մϴ�. </param>
	template< class T >
	template< class O, std::enable_if_t<!where<Object, O> && !std::is_pointer_v<O>>* >
	[[nodiscard]] inline bool RefPtr<T>::Is( O* outptr )
	{
		if ( auto ptr = dynamic_cast< NativeType<O>* >( this->ptr ); ptr )
		{
			if ( outptr )
			{
				*outptr = ptr->Unboxing();
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. </summary>
	template< class T >
	template< class O, std::enable_if_t<where<Object, O> && !where<ValueType, O>>* >
	[[nodiscard]] inline RefPtr<O> RefPtr<T>::As()
	{
#if defined( _DEBUG )
		if ( ptr == nullptr ) throw new NullReferenceException();
#endif

		RefPtr<O> outptr;
		if ( Is( ( O** )&outptr ) )
		{
			return outptr;
		}
		else
		{
			throw new InvalidCastException();
		}
	}

	/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. </summary>
	template< class T >
	template< class O, std::enable_if_t<is_refptr_v<O>>* >
	[[nodiscard]] inline O RefPtr<T>::As()
	{
#if defined( _DEBUG )
		if ( ptr == nullptr ) throw new NullReferenceException();
#endif

		O outptr;
		if ( Is<typename O::_Myt>( ( typename O::_Myt** )&outptr ) )
		{
			return outptr;
		}
		else
		{
			throw new InvalidCastException();
		}
	}

	/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. </summary>
	template< class T >
	template< class O, std::enable_if_t<where<ValueType, O>>* >
	[[nodiscard]] inline O RefPtr<T>::As()
	{
#if defined( _DEBUG )
		if ( ptr == nullptr ) throw new NullReferenceException();
#endif

		O outptr;
		if ( Is( &outptr ) )
		{
			return std::move( outptr );
		}
		else
		{
			throw new InvalidCastException();
		}
	}

	/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. </summary>
	template< class T >
	template< class O, std::enable_if_t<!where<Object, O> && !is_refptr_v<O>>* >
	[[nodiscard]] inline O RefPtr<T>::As()
	{
#if defined( _DEBUG )
		if ( ptr == nullptr ) throw new NullReferenceException();
#endif

		O outptr;
		if ( Is( ( O* )&( int& )outptr ) )
		{
			return std::move( outptr );
		}
		else
		{
			throw new InvalidCastException();
		}
	}

	/// <summary> ��ü �����͸� ������ �������� ĳ�����մϴ�. ĳ���ÿ� ������ ��� ���ܸ� �߻���Ű�� �ʰ� nullptr�� ��ȯ�մϴ�. </summary>
	template< class T >
	template< class O >
	[[nodiscard]] inline auto RefPtr<T>::TryAs()
	{
		if ( decltype( As<O>() ) ptr; Is<O>( &ptr ) )
		{
			return ptr;
		}
		else
		{
			return decltype( As<O>() )( );
		}
	}

	/// <summary> ��ü �����Ͱ� �ùٸ� �������� �ĺ��ϴ� ���� �����ɴϴ�. </summary>
	template< class T >
	[[nodiscard]] inline bool RefPtr<T>::IsValid_get() const
	{
		return ptr;
	}
	
	template< class T >
	[[nodiscard]] inline T* RefPtr<T>::operator->()
	{
#if defined( _DEBUG )
		if ( ptr == nullptr ) throw new NullReferenceException();
#endif

		return Get();
	}
	
	template< class T >
	[[nodiscard]] inline RefPtr<T>::operator bool() const
	{
		return IsValid_get();
	}
	
	template< class T >
	inline auto RefPtr<T>::operator=( RefPtr& other ) -> RefPtr&
	{
		Reset( other.Get() );
		return *this;
	}

	template< class T >
	inline auto RefPtr<T>::operator=( const RefPtr& other ) -> RefPtr&
	{
		Reset( other.ptr );
		return *this;
	}

	template< class T >
	inline auto RefPtr<T>::operator=( RefPtr&& other ) noexcept -> RefPtr&
	{
		Reset( other.Get() );
		return *this;
	}

	template< class T >
	[[nodiscard]] inline auto RefPtr<T>::operator&() -> T**
	{
		Release();
		return &ptr;
	}

	template< class T >
	template< class O >
	[[nodiscard]] inline bool RefPtr<T>::operator==( O right ) const
	{
		if ( IsValid_get() )
		{
			return ( ( T* )ptr )->operator==( ( std::remove_reference_t<std::remove_const_t<O>> )right );
		}
		else
		{
			return !right.ptr;
		}
	}

	template< class T >
	template< class O >
	[[nodiscard]] inline bool RefPtr<T>::operator!=( O right ) const
	{
		if ( IsValid_get() )
		{
			return ( ( T* )ptr )->operator!=( ( std::remove_reference_t<std::remove_const_t<O>> )right );
		}
		else
		{
			return !right.ptr;
		}
	}

	template< class T >
	template< class O >
	[[nodiscard]] inline bool RefPtr<T>::operator< ( O right ) const
	{
		if ( IsValid_get() )
		{
			return ( ( T* )ptr )->operator< ( ( std::remove_reference_t<std::remove_const_t<O>> )right );
		}
		else
		{
			return !right.ptr;
		}
	}

	template< class T >
	template< class O >
	[[nodiscard]] inline bool RefPtr<T>::operator<=( O right ) const
	{
		if ( IsValid_get() )
		{
			return ( ( T* )ptr )->operator<=( ( std::remove_reference_t<std::remove_const_t<O>> )right );
		}
		else
		{
			return !right.ptr;
		}
	}

	template< class T >
	template< class O >
	[[nodiscard]] inline bool RefPtr<T>::operator> ( O right ) const
	{
		if ( IsValid_get() )
		{
			return ( ( T* )ptr )->operator< ( ( std::remove_reference_t<std::remove_const_t<O>> )right );
		}
		else
		{
			return !right.ptr;
		}
	}

	template< class T >
	template< class O >
	[[nodiscard]] inline bool RefPtr<T>::operator>=( O right ) const
	{
		if ( IsValid_get() )
		{
			return ( ( T* )ptr )->operator>=( ( std::remove_reference_t<std::remove_const_t<O>> )right );
		}
		else
		{
			return !right.ptr;
		}
	}
}