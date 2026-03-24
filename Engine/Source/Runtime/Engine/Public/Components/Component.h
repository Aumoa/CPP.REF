// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "Component.gen.h"

namespace Ayla
{
	class Actor;
	class GameObject;
	class Transform;

	ACLASS()
	class ENGINE_API Component : public SerializableObject
	{
		GENERATED_BODY()
		friend class Actor;

	private:
		bool m_Disposed = false;
		Actor* m_ActorPtr = nullptr;

	public:
		Component();
		virtual ~Component() noexcept override;

		Actor* GetActor() const
		{
			ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());
			return m_ActorPtr;
		}

		GameObject* GetGameObject() const;
		Transform* GetTransform() const;
	};
}