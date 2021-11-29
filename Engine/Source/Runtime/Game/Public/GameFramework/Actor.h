// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "SubclassOf.h"
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
	class SActorTickFunction : public STickFunction
	{
		GENERATED_BODY(SActorTickFunction)

	private:
		AActor* const _target = nullptr;

	public:
		inline SActorTickFunction(AActor* target) : Super()
			, _target(target)
		{
		}

		inline AActor* GetTarget() const
		{
			return _target;
		}

		virtual void ExecuteTick(float elapsedTime) override;
	};

public:
	SActorTickFunction PrimaryActorTick;

private:
	uint8 _bActive : 1;
	uint8 _bHasBegunPlay : 1;

public:
	AActor();

	virtual void TickActor(float elapsedTime, SActorTickFunction* tickFunction);

	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void Tick(float elapsedTime);

	void SetActive(bool bActive);
	inline bool IsActive() const { return _bActive; }
	inline bool HasBegunPlay() const { return _bHasBegunPlay; }

	void DestroyActor();

public:
	MulticastEvent<AActor, void()> Activated;
	MulticastEvent<AActor, void()> Inactivated;

public:
	virtual void PostInitializedComponents();
	void DispatchBeginPlay();
	void DispatchEndPlay();

private:
	std::set<SActorComponent*> _Components;

public:
	void AddOwnedComponent(SActorComponent* InComponent);
	const std::set<SActorComponent*>& GetOwnedComponents();
	SActorComponent* GetComponentByClass(SubclassOf<SActorComponent> InComponentClass);

	template<std::derived_from<SActorComponent> T>
	T* GetComponentAs()
	{
		if constexpr (std::derived_from<T, SSceneComponent>)
		{
			for (auto& Component : GetSceneComponents())
			{
				if (auto* Ptr = dynamic_cast<T*>(Component))
				{
					return Ptr;
				}
			}

			return nullptr;
		}
		else  // T is derived from SActorComponent.
		{
			for (auto& Component : _Components)
			{
				if (auto* Ptr = dynamic_cast<T*>(Component))
				{
					return Ptr;
				}
			}

			return nullptr;
		}
	}

private:
	SSceneComponent* _RootComponent = nullptr;

public:
	std::shared_ptr<SSceneComponent> SetRootComponent(SSceneComponent* InRootComponent);
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