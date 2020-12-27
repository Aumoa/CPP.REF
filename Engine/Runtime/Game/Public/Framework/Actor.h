// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <set>
#include <list>
#include <tuple>
#include <array>

namespace SC::Runtime::Game::Components
{
	class ActorComponent;
	class SceneComponent;
}

namespace SC::Runtime::Game::Framework
{
	class GAME_API AActor : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = AActor;

	private:
		std::map<Components::ActorComponent*, Core::TRefPtr<Components::ActorComponent>> ownedComponents;
		std::map<size_t, std::list<Components::ActorComponent*>> hierarchy;
		Core::TRefPtr<Components::SceneComponent> rootComponent;

	public:
		AActor();
		~AActor() override;

		template<class T, class... TArgs> requires Core::TIsAssignable<T*, Components::ActorComponent*> && Core::THasConstructor<T, TArgs...>
		inline T* AddComponent(TArgs&&... args);
		template<class T> requires Core::TIsAssignable<T*, Components::ActorComponent*>
		inline bool RemoveComponent();

		vs_property(Components::SceneComponent*, RootComponent);
		Components::SceneComponent* RootComponent_get() const;
		void RootComponent_set(Components::SceneComponent* value);

	private:
		bool AddComponentInternal(Core::TRefPtr<Components::ActorComponent>&& assign_ptr, const size_t* hierarchy, size_t num);
		bool RemoveComponentInternal(size_t hash_code);

		template<class... TArgs, size_t... Indices>
		inline static std::array<size_t, sizeof...(TArgs)> CalcHierarchy2(const std::tuple<TArgs...>& tuple, std::index_sequence<Indices...>);

		template<class T>
		inline static consteval auto CalcHierarchy()
		{
			if constexpr (std::is_same_v<T, Components::ActorComponent>)
			{
				return std::tuple<size_t>(Core::TUniqueType<T>::HashCode);
			}
			else
			{
				return CalcHierarchy<typename T::Super>(std::tuple<size_t>(Core::TUniqueType<T>::HashCode), std::make_index_sequence<1>{ });
			}
		}

		template<class T, class... TRest, size_t... Indices>
		inline static consteval auto CalcHierarchy(const std::tuple<TRest...>& rest, std::index_sequence<Indices...>)
		{
			if constexpr (std::is_same_v<T, Components::ActorComponent>)
			{
				return std::tuple(Core::TUniqueType<T>::HashCode, std::get<Indices>(rest)...);
			}
			else
			{
				return CalcHierarchy<typename T::Super>(std::tuple(Core::TUniqueType<T>::HashCode, std::get<Indices>(rest)...), std::make_index_sequence<sizeof...(TRest) + 1>{ });
			}
		}
	};
}

#include "Actor.inl"