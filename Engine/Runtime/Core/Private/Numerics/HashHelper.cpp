// Copyright 2020 Aumoa.lib. All right reserved.

#include "HashHelper.h"

using namespace std;

template<class T, size_t... Seq>
static size_t GetHashCodeInlineHelper(T value, std::index_sequence<Seq...>&&)
{
	size_t* Array = reinterpret_cast<size_t*>(&value);
	return (... + Array[Seq]);
}

size_t HashHelper::GetHashCode(float value)
{
	constexpr size_t NumArray = (sizeof(float) + sizeof(size_t) - 1) / sizeof(size_t);
	return GetHashCodeInlineHelper(value, make_index_sequence<NumArray>{ });
}