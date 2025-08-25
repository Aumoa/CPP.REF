// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Component.h"
#include "Behaviour.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API Behaviour : public Component
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		bool m_Enabled = true;

	public:
		Behaviour();
		virtual ~Behaviour() noexcept override;

		void SetEnabled(bool value);
		bool IsEnabled() const noexcept { return m_Enabled; }
	};
}