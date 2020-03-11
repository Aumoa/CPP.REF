#pragma once

namespace SC
{
	namespace Details
	{
		template< class T, class = void >
		class HasOperator : public std::false_type
		{

		};

		template< class T >
		class HasOperator<T, std::void_t<decltype( std::declval<std::wostream&>() << std::declval<T>() )>> : public std::true_type
		{

		};
	}
}