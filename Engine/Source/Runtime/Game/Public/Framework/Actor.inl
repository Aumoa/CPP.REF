﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<class T, class... TArgs> requires TIsAssignable<T*, GActorComponent*> && THasConstructor<T, TArgs...>
inline T* AActor::AddComponent(TArgs&&... args)
{
	using namespace std;

	TRefPtr<T> obj = NewObject<T>(args...);
	T* ptr = obj.Get();

	constexpr auto const_hierarchy = CalcHierarchy<T>();
	constexpr auto num = tuple_size<decltype(const_hierarchy)>::value;
	auto hierarchy = CalcHierarchy2(const_hierarchy, make_index_sequence<num>{ });

	if (AddComponentInternal(move(obj), hierarchy.data(), hierarchy.size()))
	{
		return ptr;
	}
	else
	{
		return nullptr;
	}
}

template<class T> requires TIsAssignable<T*, GActorComponent*>
inline bool AActor::RemoveComponent()
{
	return RemoveComponentInternal(TUniqueType<T>::HashCode);
}

template<class T> requires TIsAssignable<T*, GActorComponent*>
inline T* AActor::GetComponent() const
{
	constexpr size_t HashCode = TUniqueType<T>::HashCode;

	auto it = hierarchy.find(HashCode);
	if (it == hierarchy.end())
	{
		return nullptr;
	}

	return Cast<T>(it->second.front());
}

template<class T> requires TIsAssignable<T*, GActorComponent*>
inline std::list<T*> AActor::GetComponents() const
{
	constexpr size_t HashCode = TUniqueType<T>::HashCode;
	
	std::list<T*> items;

	auto it = hierarchy.find(HashCode);
	if (it != hierarchy.end())
	{
		for (auto& item : it->second)
		{
			items.emplace_back(Cast<T>(item));
		}
	}

	return items;
}

template<class... TArgs, size_t... Indices>
inline std::array<size_t, sizeof...(TArgs)> AActor::CalcHierarchy2(const std::tuple<TArgs...>& tuple, std::index_sequence<Indices...>)
{
	using namespace std;
	array<size_t, sizeof...(TArgs)> vec = { get<Indices>(tuple)... };
	return move(vec);
}