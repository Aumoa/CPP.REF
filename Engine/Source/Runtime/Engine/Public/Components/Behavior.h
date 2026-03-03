// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Component.h"
#include "Behavior.gen.h"

namespace Ayla
{
	class Actor;

	ACLASS()
	class ENGINE_API Behavior : public Component
	{
		GENERATED_BODY()
		friend Actor;

	private:
		bool m_Enabled = true;

	public:
		Behavior();
		virtual ~Behavior() noexcept override;

		AFUNCTION()
		void SetEnabled(bool value);
		AFUNCTION()
		bool IsEnabled() const;

	protected:
		AFUNCTION()
		virtual void OnEnable();
		AFUNCTION()
		virtual void OnDisable();
	};
}