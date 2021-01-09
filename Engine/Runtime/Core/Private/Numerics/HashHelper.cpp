// Copyright 2020 Aumoa.lib. All right reserved.

#include "HashHelper.h"

using namespace std;

#if _WIN64
constexpr static size_t FNV_Prime = 1099511628211ULL;
constexpr static size_t FNV_Basis = 14695981039346656037ULL;
#else
constexpr static size_t FNV_Prime = 16777619UL;
constexpr static size_t FNV_Basis = 2166136261UL;
#endif

static size_t GetHashCode(size_t N, const char* S, size_t H = FNV_Basis)
{
	return N > 0 ? GetHashCode(N - 1, S + 1, (H ^ *S) * FNV_Prime) : H;
}

size_t HashHelper::GetHashCode(float value)
{
	return ::GetHashCode(sizeof(float), (const char*)&value);
}