#pragma once

namespace SC
{
	template< class T >
	struct is_refptr
	{
		static constexpr bool value = false;
	};

	template< class T >
	struct is_refptr<RefPtr<T>>
	{
		static constexpr bool value = true;
	};

	template< class T >
	constexpr bool is_refptr_v = is_refptr<T>::value;

	template< class T >
	struct remove_refptr
	{
		using type = T;
	};

	template< class T >
	struct remove_refptr<RefPtr<T>>
	{
		using type = T;
	};

	template< class T >
	using remove_refptr_t = typename remove_refptr<T>::type;
}