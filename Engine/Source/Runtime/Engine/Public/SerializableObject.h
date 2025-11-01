// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API SerializableObject : public Object
	{
		GENERATED_BODY()

	private:
		bool m_Disposed = false;
		String m_Name;

	public:
		SerializableObject();
		virtual ~SerializableObject() noexcept override;

		void MarkDirty();

		AFUNCTION()
		void SetName(String name);
		AFUNCTION()
		String GetName() const;

		AFUNCTION()
		static void Destroy(SharedPtr<SerializableObject> obj);
	};
}