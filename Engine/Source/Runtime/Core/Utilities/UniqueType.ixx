// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:UniqueType;

export
{
	template<class T>
	struct UniqueType
	{
		constexpr static size_t FNV_Prime = 1099511628211ULL;
		constexpr static size_t FNV_Basis = 14695981039346656037ULL;

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