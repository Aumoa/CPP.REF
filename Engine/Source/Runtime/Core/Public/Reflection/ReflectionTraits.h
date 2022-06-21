// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <vector>

class Object;

namespace libty::reflect
{
	struct constructor_t
	{
		using function_t = void*(*)(std::vector<void*>);

		function_t fnc;
		size_t hash;

		inline constructor_t(function_t fnc, size_t hash)
			: fnc(fnc)
			, hash(hash)
		{
		}

		inline operator function_t() const
		{
			return fnc;
		}
	};
}