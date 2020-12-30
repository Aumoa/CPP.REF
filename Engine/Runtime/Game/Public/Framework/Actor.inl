// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

template<class T, class... TArgs> requires TIsAssignable<T*, ActorComponent*> && THasConstructor<T, TArgs...>
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

template<class T> requires TIsAssignable<T*, ActorComponent*>
inline bool AActor::RemoveComponent()
{
	return RemoveComponentInternal(TUniqueType<T>::HashCode);
}

template<class T> requires TIsAssignable<T*, ActorComponent*>
inline T* AActor::GetComponent() const
{
	constexpr size_t HashCode = TUniqueType<T>::HashCode;

	auto it = hierarchy.find(HashCode);
	if (it == hierarchy.end())
	{
		if constexpr (!std::is_base_of_v<SceneComponent, T>)
		{
			return nullptr;
		}

		if (!rootComponent.IsValid)
		{
			return nullptr;
		}

		std::stack<SceneComponent*> roots;
		roots.push(rootComponent.Get());

		while (!roots.empty())
		{
			std::stack<SceneComponent*> stack_pop;
			std::stack<SceneComponent*> stack_push;

			roots.swap(stack_pop);
			while (!stack_pop.empty())
			{
				auto parent = stack_pop.top();
				stack_pop.pop();

				if (auto ptr = Cast<T>(parent))
				{
					return ptr;
				}

				for (auto child : parent->GetChildComponents())
				{
					stack_push.push(child.Get());
				}
			}
			roots.swap(stack_push);
		}
	}

	return it->second.front();
}

template<class T> requires TIsAssignable<T*, ActorComponent*>
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

	if constexpr (std::is_base_of_v<SceneComponent, T>)
	{
		std::stack<SceneComponent*> roots;
		roots.push(rootComponent.Get());

		while (!roots.empty())
		{
			std::stack<SceneComponent*> stack_pop;
			std::stack<SceneComponent*> stack_push;

			roots.swap(stack_pop);
			while (!stack_pop.empty())
			{
				auto parent = stack_pop.top();
				stack_pop.pop();

				if (auto ptr = Cast<T>(parent))
				{
					items.emplace_back(ptr);
				}

				for (auto child : parent->GetChildComponents())
				{
					stack_push.push(child.Get());
				}
			}
			roots.swap(stack_push);
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