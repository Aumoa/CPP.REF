// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "GameObject.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API GameObject : public SerializableObject
	{
		GENERATED_BODY()

	private:
		bool m_Active;

	public:
		GameObject();
		virtual ~GameObject() noexcept override;

		AFUNCTION()
		void SetActive(bool active);
		AFUNCTION()
		bool IsActiveSelf();
	};
}