// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "Ticking/TickFunction.h"
#include "Components/SceneComponent.h"
#include <set>

DECLARE_LOG_CATEGORY(GAME_API, LogActor);

class SActorComponent;
class SSceneComponent;

class GAME_API AActor : public SGameObject
{
	GENERATED_BODY(AActor)

private:
	class ActorTickFunction : public TickFunction
	{
	private:
		AActor* Target = nullptr;

	public:
		ActorTickFunction(AActor* InTarget);

		AActor* GetTarget() const;
		virtual void ExecuteTick(float InDeltaTime) override;
	};

public:
	ActorTickFunction PrimaryActorTick;

private:
	uint8 bActive : 1;
	uint8 bHasBegunPlay : 1;

public:
	AActor();

	virtual void TickActor(float InDeltaTime, ActorTickFunction* ActorTickFunction);

	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(float InDeltaTime);

	void SetActive(bool bActive);
	inline bool IsActive() const { return bActive; }
	inline bool HasBegunPlay() const { return bHasBegunPlay; }

	void DestroyActor();

public:
	DECLARE_MULTICAST_EVENT(ActivationChangedEvent);
	ActivationChangedEvent Activated;
	ActivationChangedEvent Inactivated;

public:
	virtual void PostInitializedComponents();
	void DispatchBeginPlay();
	void DispatchEndPlay();

private:
	SPROPERTY(OwnedComponents)
	std::set<SActorComponent*> OwnedComponents;
	SPROPERTY(RootComponent)
	SSceneComponent* RootComponent = nullptr;

public:
	void AddOwnedComponent(SActorComponent* InComponent);
	const std::set<SActorComponent*>& GetOwnedComponents();
	SActorComponent* GetComponentByClass(SubclassOf<SActorComponent> InComponentClass);

	template<std::derived_from<SActorComponent> T>
	T* GetComponentAs()
	{
		if constexpr (std::derived_from<T, SSceneComponent>)
		{
			std::vector<SSceneComponent*> InlineComponentArray;
			InlineComponentArray.reserve(16);
			InlineComponentArray.emplace_back(RootComponent);
			
			while (InlineComponentArray.size())
			{
				SSceneComponent* Back = InlineComponentArray.back();
				InlineComponentArray.erase(InlineComponentArray.end() - 1);

				if (Back->GetType()->IsDerivedFrom<T>())
				{
					return Cast<T>(Back);
				}
				else
				{
					const auto& ChildComponentArray = Back->GetChildComponents();
					InlineComponentArray.insert(InlineComponentArray.end(), ChildComponentArray.begin(), ChildComponentArray.end());
				}
			}

			return nullptr;
		}
		else  // T is derived from SActorComponent.
		{
			for (auto& Component : OwnedComponents)
			{
				if (Component->GetType()->IsDerivedFrom<T>())
				{
					return Component;
				}
			}

			return nullptr;
		}
	}

public:
	void SetRootComponent(SSceneComponent* InRootComponent);
	SSceneComponent* GetRootComponent();
	template<std::derived_from<SSceneComponent> T>
	T* GetRootComponentAs() { return dynamic_cast<T*>(GetRootComponent()); }
	std::vector<SSceneComponent*> GetSceneComponents();

public:
	void SetActorLocation(const Vector3& location);
	Vector3 GetActorLocation() const;
	void SetActorRotation(const Quaternion& rotation);
	Quaternion GetActorRotation() const;
};