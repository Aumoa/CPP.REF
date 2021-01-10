// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <set>
#include <list>
#include <tuple>
#include <array>
#include <chrono>
#include <stack>
#include "TickFunction.h"
#include "Components/SceneComponent.h"

class World;

class GAME_API AActor : virtual public Object
{
	struct ActorTickFunction : public TickFunction
	{
		using Super = TickFunction;
		using This = ActorTickFunction;

		AActor* Target;

		ActorTickFunction();
		~ActorTickFunction();

		void ExecuteTick(Seconds deltaTime) override;
	};

public:
	using Super = Object;
	using This = AActor;

	using ComponentAddedDelegate = TMulticastDelegate<void(ActorComponent*)>;
	using ComponentRemovedDelegate = TMulticastDelegate<void(ActorComponent*)>;

private:
	std::map<ActorComponent*, TRefPtr<ActorComponent>> ownedComponents;
	std::map<size_t, std::list<ActorComponent*>> hierarchy;
	SceneComponent* rootComponent;
	ActorTickFunction primaryActorTick;
	bool bActorTickEnabled : 1;
	bool bActorHasBegunPlay : 1;
	World* world;

public:
	AActor();
	~AActor() override;

	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(Seconds deltaTime);

	template<class T, class... TArgs> requires TIsAssignable<T*, ActorComponent*> && THasConstructor<T, TArgs...>
	inline T* AddComponent(TArgs&&... args);
	template<class T> requires TIsAssignable<T*, ActorComponent*>
	inline bool RemoveComponent();

	template<class T> requires TIsAssignable<T*, ActorComponent*>
	inline T* GetComponent() const;
	template<class T> requires TIsAssignable<T*, ActorComponent*>
	inline std::list<T*> GetComponents() const;

	World* GetWorld() const;
	void SetWorld(World* inWorld);
	Transform GetActorTransform() const;
	void SetActorTransform(const Transform& value);

	vs_property(SceneComponent*, RootComponent);
	SceneComponent* RootComponent_get() const;
	void RootComponent_set(SceneComponent* value);
	vs_property_get(ActorTickFunction&, PrimaryActorTick);
	ActorTickFunction& PrimaryActorTick_get();
	vs_property(bool, ActorTickEnabled);
	bool ActorTickEnabled_get() const;
	void ActorTickEnabled_set(bool value);
	vs_property_get(bool, HasBegunPlay);
	bool HasBegunPlay_get() const;

	ComponentAddedDelegate ComponentAdded;
	ComponentRemovedDelegate ComponentRemoved;

private:
	bool AddComponentInternal(TRefPtr<ActorComponent>&& assign_ptr, const size_t* hierarchy, size_t num);
	bool RemoveComponentInternal(size_t hash_code);

	template<class... TArgs, size_t... Indices>
	inline static std::array<size_t, sizeof...(TArgs)> CalcHierarchy2(const std::tuple<TArgs...>& tuple, std::index_sequence<Indices...>);

	template<class T>
	inline static consteval auto CalcHierarchy()
	{
		if constexpr (std::is_same_v<T, ActorComponent>)
		{
			return std::tuple<size_t>(TUniqueType<T>::HashCode);
		}
		else
		{
			return CalcHierarchy<typename T::Super>(std::tuple<size_t>(TUniqueType<T>::HashCode), std::make_index_sequence<1>{ });
		}
	}

	template<class T, class... TRest, size_t... Indices>
	inline static consteval auto CalcHierarchy(const std::tuple<TRest...>& rest, std::index_sequence<Indices...>)
	{
		if constexpr (std::is_same_v<T, ActorComponent>)
		{
			return std::tuple(TUniqueType<T>::HashCode, std::get<Indices>(rest)...);
		}
		else
		{
			return CalcHierarchy<typename T::Super>(std::tuple(TUniqueType<T>::HashCode, std::get<Indices>(rest)...), std::make_index_sequence<sizeof...(TRest) + 1>{ });
		}
	}
};

#include "Actor.inl"