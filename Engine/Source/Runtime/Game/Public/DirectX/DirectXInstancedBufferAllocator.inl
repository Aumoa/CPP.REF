// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<class T, class... TConstructorArgs>
T& DirectXInstancedBufferAllocator::Emplace(size_t index, TConstructorArgs&&... inArgs)
{
	if (sizeof(T) != ItemSize)
	{
		throw Exception(L"Type cast failed.");
	}

	T* cast_ptr= (T*)bufferPtr;
	new(cast_ptr + index) T(std::forward<TConstructorArgs>(inArgs)...);
	return cast_ptr[index];
}