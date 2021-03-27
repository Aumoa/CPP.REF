// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "GameObject.h"

#include "TickFunction.h"
#include "Components/SceneComponent.h"

class World;
enum class EComponentMobility;

class GAME_API AActor : public GGameObject, virtual public ITickFunctionObject
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

	using ComponentAddedDelegate = TMulticastDelegate<void(GActorComponent*)>;
	using ComponentRemovedDelegate = TMulticastDelegate<void(GActorComponent*)>;

private:
	std::map<GActorComponent*, TRefPtr<GActorComponent>> ownedComponents;
	std::map<size_t, std::list<GActorComponent*>> hierarchy;
	GSceneComponent* rootComponent;
	ActorTickFunction primaryActorTick;
	bool bActorTickEnabled : 1;
	bool bActorHasBegunPlay : 1;

public:
	AActor();
	~AActor() override;

	virtual TickFunction* GetTickFunction() override;
	virtual void AddPrerequisiteObject(ITickFunctionObject* inObject) override;
	virtual void RemovePrerequisiteObject(ITickFunctionObject* inObject) override;

	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void TickActor(Seconds deltaTime);

	template<class T, class... TArgs> requires TIsAssignable<T*, GActorComponent*> && THasConstructor<T, TArgs...>
	inline T* AddComponent(TArgs&&... args);
	template<class T> requires TIsAssignable<T*, GActorComponent*>
	inline bool RemoveComponent();

	template<class T> requires TIsAssignable<T*, GActorComponent*>
	inline T* GetComponent() const;
	template<class T> requires TIsAssignable<T*, GActorComponent*>
	inline std::list<T*> GetComponents() const;

	Transform GetActorTransform() const;
	void SetActorTransform(const Transform& value);

	Vector3 GetActorLocation() const;
	void SetActorLocation(const Vector3& value);
	Vector3 GetActorScale() const;
	void SetActorScale(const Vector3& value);
	Quaternion GetActorRotation() const;
	void SetActorRotation(const Quaternion& value);

	EComponentMobility GetMobility() const;
	void SetMobility(EComponentMobility inRootMobility);

	vs_property(GSceneComponent*, RootComponent);
	vs_property_get(ActorTickFunction&, PrimaryActorTick);
	vs_property(bool, ActorTickEnabled);
	vs_property_get(bool, HasBegunPlay);

	ComponentAddedDelegate ComponentAdded;
	ComponentRemovedDelegate ComponentRemoved;

protected:
	virtual void Tick(Seconds deltaTime);

private:
	bool AddComponentInternal(TRefPtr<GActorComponent>&& assign_ptr, const size_t* hierarchy, size_t num);
	bool RemoveComponentInternal(size_t hash_code);

	template<class... TArgs, size_t... Indices>
	inline static std::array<size_t, sizeof...(TArgs)> CalcHierarchy2(const std::tuple<TArgs...>& tuple, std::index_sequence<Indices...>);

	template<class T>
	inline static consteval auto CalcHierarchy()
	{
		if constexpr (std::is_same_v<T, GActorComponent>)
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
		if constexpr (std::is_same_v<T, GActorComponent>)
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