// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "NamedObject.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API NamedObject : public SerializableObject
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		String m_Name;

	public:
		NamedObject();
		virtual ~NamedObject() noexcept override;

		void SetName(String name);
		String GetName() const noexcept { return m_Name; }
	};
}