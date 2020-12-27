// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"

namespace SC::Runtime::Core
{
	template<class T>
	struct TUniqueType
	{
#if _WIN64
		constexpr static size_t FNV_Prime = 1099511628211ULL;
		constexpr static size_t FNV_Basis = 14695981039346656037ULL;
#else
		constexpr static size_t FNV_Prime = 16777619UL;
		constexpr static size_t FNV_Basis = 2166136261UL;
#endif

		consteval static decltype(auto) GetUniqueName()
		{
			return __FUNCSIG__;
		}

		consteval static size_t GetHashCode(size_t N, const char* S, size_t H = FNV_Basis)
		{
			return N > 0 ? GetHashCode(N - 1, S + 1, (H ^ *S) * FNV_Prime) : H;
		}

		template<size_t N>
		consteval static size_t GetHashCode(const char(&S)[N])
		{
			return GetHashCode(N, S);
		}

		constexpr static decltype(auto) UniqueName = GetUniqueName();
		constexpr static auto HashCode = GetHashCode(UniqueName);
	};
}