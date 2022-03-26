// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::Core::Reflection
{
	class GCNewBinder
	{
	public:
		constexpr GCNewBinder()
		{
		}

		template<class T>
		T* operator << (T* Ptr);
	};
}

#define gcnew libty::Core::Reflection::GCNewBinder() << new