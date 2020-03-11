#pragma once

namespace SC
{
	template< class T >
	constexpr bool where_is_class = where<Object, T> && !where<ValueType, T>;

	template< class T >
	constexpr bool where_is_value = where<ValueType, T>;

	template< class T >
	constexpr bool where_is_native = !where<Object, T>;
}