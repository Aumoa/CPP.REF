#pragma once

namespace SC
{
	template< class Base, class Derived >
	constexpr bool where = std::is_base_of_v<Base, Derived>;
}