// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "TickFunction.h"

class GAME_API ActorComponent : public Object
{
	struct ComponentTickFunction : public TickFunction
	{
		using Super = TickFunction;
		using This = ComponentTickFunction;

		ActorComponent* Target;

		ComponentTickFunction();
		~ComponentTickFunction();

		void ExecuteTick(std::chrono::duration<double> deltaTime) override;
	};

public:
	using Super = Object;
	using This = ActorComponent;

private:
	ComponentTickFunction primaryComponentTick;
	bool bComponentTickEnabled : 1;
	bool bComponentHasBegunPlay : 1;

public:
	ActorComponent();
	~ActorComponent() override;

	virtual void BeginPlay();
	virtual void EndPlay();
	virtual void TickComponent(std::chrono::duration<double> deltaTime);

	vs_property_get(ComponentTickFunction&, PrimaryComponentTick);
	ComponentTickFunction& PrimaryComponentTick_get();
	vs_property(bool, ComponentTickEnabled);
	bool ComponentTickEnabled_get() const;
	void ComponentTickEnabled_set(bool value);
	vs_property_get(bool, HasBegunPlay);
	bool HasBegunPlay_get() const;
};