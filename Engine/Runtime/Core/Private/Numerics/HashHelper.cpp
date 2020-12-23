// Copyright 2020 Aumoa.lib. All right reserved.

#include "HashHelper.h"

using namespace SC::Runtime::Core;
using namespace std;

template<class T, size_t... Seq>
static size_t GetHashCodeInlineHelper(T value, std::index_sequence<Seq...>&&)
{
	constexpr size_t NumArray = sizeof(T) / sizeof(size_t);
	size_t* Array = reinterpret_cast<size_t*>(&value);
	return (... + Array[Seq]);
}

size_t HashHelper::GetHashCode(double value)
{
	constexpr size_t NumArray = sizeof(double) / sizeof(size_t);
	return GetHashCodeInlineHelper(value, make_index_sequence<NumArray>{ });
}