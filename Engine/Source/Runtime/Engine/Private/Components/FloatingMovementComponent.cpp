// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Components/FloatingMovementComponent.h"
#include "Components/Transform.h"
#include "InputSystem/InputManager.h"
#include "Misc/DefaultVectors.h"
#include "GameContext.h"

namespace Ayla
{
	FloatingMovementComponent::FloatingMovementComponent()
	{
	}

	FloatingMovementComponent::~FloatingMovementComponent() noexcept
	{
	}

	void FloatingMovementComponent::Tick(TickTiming timing, const TimeSpan& deltaTime)
	{
		Super::Tick(timing, deltaTime);

		Vector3F velocity = GetVelocity();
		ApplyControlInputToVelocity(deltaTime, &velocity);

		auto deltaLocation = velocity * (float)deltaTime.GetTotalSeconds();
		if (!deltaLocation.NearlyEquals(Vector3F::Zero(), Math::SmallNumber))
		{
			Vector3F oldLocation;
			QuaternionF oldRotation;
			GetTransform()->GetPositionAndRotation(&oldLocation, &oldRotation);

			Vector3F newLocation = oldLocation + deltaLocation;
			GetTransform()->SetPositionAndRotation(newLocation, oldRotation);

			velocity = (newLocation - oldLocation) / (float)deltaTime.GetTotalSeconds();
		}

		SetVelocity(velocity);
	}

	void FloatingMovementComponent::ApplyControlInputToVelocity(const TimeSpan& deltaTime, Vector3F* velocity) const
	{
		auto* im = GameContext::GetInputManager();
		bool w = im->GetKey(KeyCode::W);
		bool a = im->GetKey(KeyCode::A);
		bool s = im->GetKey(KeyCode::S);
		bool d = im->GetKey(KeyCode::D);
		float forwardAcceleration = (w ? 1.0f : 0.0f) - (s ? 1.0f : 0.0f);
		float rightAcceleration = (d ? 1.0f : 0.0f) - (a ? 1.0f : 0.0f);
		auto controlAcceleration = DefaultVectors<3>::Forward * forwardAcceleration + DefaultVectors<3>::Right * rightAcceleration;

		const float analogInputModifier = (Vector<>::LengthSq(controlAcceleration) > 0.f ? Vector<>::Length(controlAcceleration) : 0.f);
		const float maxPawnSpeed = GetMaxSpeed() * analogInputModifier;
		const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(maxPawnSpeed);

		if (analogInputModifier > 0.f && !bExceedingMaxSpeed)
		{
			if (Vector<>::LengthSq(*velocity) > 0.f)
			{
				const float timeScale = Math::Clamp((float)(deltaTime.GetTotalSeconds() * m_TurningBoost), 0.f, 1.f);
				*velocity = *velocity + (controlAcceleration * Vector<>::Length(*velocity) - *velocity) * timeScale;
			}
		}
		else
		{
			if (Vector<>::LengthSq(*velocity) > 0.f)
			{
				const Vector3F OldVelocity = *velocity;
				const float VelSize = Math::Max(Vector<>::Length(*velocity) - Math::Abs(m_Deceleration) * (float)deltaTime.GetTotalSeconds(), 0.f);
				*velocity = Vector<>::SafeNormalize(*velocity) * VelSize;

				if (bExceedingMaxSpeed && Vector<>::LengthSq(*velocity) < Math::Square(maxPawnSpeed))
				{
					*velocity = Vector<>::SafeNormalize(OldVelocity) * maxPawnSpeed;
				}
			}
		}

		const float NewMaxSpeed = (IsExceedingMaxSpeed(maxPawnSpeed)) ? Vector<>::Length(*velocity) : maxPawnSpeed;
		*velocity += controlAcceleration * Math::Abs(m_Acceleration) * (float)deltaTime.GetTotalSeconds();
		*velocity = Vector<>::ClampToMaxSize(*velocity, NewMaxSpeed);
	}
}