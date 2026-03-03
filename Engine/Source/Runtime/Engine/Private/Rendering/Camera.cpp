// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/Camera.h"
#include "Actors/GameObject.h"
#include "SceneManagement/Scene.h"
#include "Rendering/MinimalViewInfo.h"
#include "Components/Transform.h"

namespace Ayla
{
	Camera::Camera() : Super()
	{
	}

	Camera::~Camera() noexcept
	{
	}

	void Camera::GetMinimalViewInfo(MinimalViewInfo* output) const
	{
		auto gameObject = (GameObject*)GetActor();
		auto transform = gameObject->GetTransform();
		transform->GetPositionAndRotation(&output->Position, &output->Rotation);
		output->FieldOfView = 60.0f;
		output->AspectRatio = std::nullopt;
	}

	void Camera::OnEnable()
	{
		auto gameObject = (GameObject*)GetActor();
		gameObject->GetScene()->AddCameraComponent(this);
	}

	void Camera::OnDisable()
	{
		auto gameObject = (GameObject*)GetActor();
		gameObject->GetScene()->RemoveCameraComponent(this);
	}
}