// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Game::Framework
{
	template<class T, class... TArgs> requires Core::TIsAssignable<T*, Components::ActorComponent*> && Core::THasConstructor<T, TArgs...>
	inline T* AActor::AddComponent(TArgs&&... args)
	{
		using namespace SC::Runtime::Core;
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

	template<class T> requires Core::TIsAssignable<T*, Components::ActorComponent*>
	inline bool AActor::RemoveComponent()
	{
		return RemoveComponentInternal(Core::TUniqueType<T>::HashCode);
	}

	template<class... TArgs, size_t... Indices>
	inline std::array<size_t, sizeof...(TArgs)> AActor::CalcHierarchy2(const std::tuple<TArgs...>& tuple, std::index_sequence<Indices...>)
	{
		using namespace std;
		array<size_t, sizeof...(TArgs)> vec = { get<Indices>(tuple)... };
		return move(vec);
	}
}