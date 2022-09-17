// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Components/SceneComponent.h"
#include "SceneComponent.gen.cpp"

constexpr LogCategory LogSceneComponent(TEXT("LogSceneComponent"));

SceneComponent::SceneComponent()
{
}

void SceneComponent::UpdateChildTransforms()
{
	for (SceneComponent* Child : Childs)
	{
		Child->UpdateComponentToWorld();
	}
}

void SceneComponent::UpdateComponentToWorld()
{
	if (GetAttachParent() == nullptr)
	{
		RelativeTransform = Transform::Identity();
		return;
	}

	if (GetAttachSocketName() != TEXT(""))
	{
		RelativeTransform = GetAttachParent()->GetSocketTransform(GetAttachSocketName());
	}
	else
	{
		RelativeTransform = GetAttachParent()->GetComponentTransform();
	}

	UpdateWorldTransform();
}

Transform SceneComponent::GetSocketTransform(const String& InSocketName, EComponentTransformSpace InSpace) const
{
	Log::Error(LogSceneComponent, TEXT("SceneComponent::GetSocketName() called. SceneComponent have not any sockets. Use override this function and provide correct socket transform."));

	switch (InSpace)
	{
	case EComponentTransformSpace::World:
		return GetComponentTransform();
	default:
		return GetRelativeTransform();
	}
}

bool SceneComponent::MoveComponent(const Vector3& InMoveDelta, const Quaternion& InNewRotation, EComponentTransformSpace InSpace)
{
	Quaternion OldRotation = InSpace == EComponentTransformSpace::World ? GetComponentRotation() : GetRotation();
	if (InMoveDelta.NearlyEquals(Vector3::Zero(), MathEx::SmallNumber) && OldRotation.NearlyEquals(InNewRotation, MathEx::SmallNumber))
	{
		// MoveDelta and NewRotation is nearly equals to previous component transform.
		// Skip moving and return state indicating that be not moved.
		return false;
	}

	Vector3 RelativeLocation;
	Quaternion RelativeRotation;

	if (InSpace == EComponentTransformSpace::World && GetAttachParent() != nullptr)
	{
		// Transform unit is only calculate from local space.
		// Therefore, will convert world space unit to local space unit and apply it.
		Vector3 NewLocation = GetComponentLocation() + InMoveDelta;
		Quaternion NewRotation = InNewRotation;

		auto WorldTransform = Transform(NewLocation, GetComponentScale(), NewRotation);
		auto RelativeTransform = Transform::GetRelativeTransform(WorldTransform, GetAttachParent()->GetSocketTransform(GetAttachSocketName()));

		RelativeLocation = RelativeTransform.Translation;
		RelativeRotation = RelativeTransform.Rotation;
	}
	else
	{
		RelativeLocation = GetLocation() + InMoveDelta;
		RelativeRotation = InNewRotation;
	}

	RelativeTransform.Translation = RelativeLocation;
	RelativeTransform.Rotation = RelativeRotation;
	UpdateWorldTransform();

	return true;
}

void SceneComponent::AttachToComponent(SceneComponent* AttachTo)
{
	AttachToSocket(AttachTo, TEXT(""));
}

void SceneComponent::AttachToSocket(SceneComponent* AttachTo, const String& socketName)
{
	if (AttachTo == nullptr)
	{
		Log::Warning(LogSceneComponent, TEXT("AttachTo is nullptr. First argument of AttachToComponent function must not be nullptr. Abort."));
		return;
	}

	if (Attachment.AttachmentRoot.Get() == AttachTo && Attachment.SocketName == socketName)
	{
		Log::Info(LogSceneComponent, TEXT("Component is already attach to desired target. Abort."));
		return;
	}

	if (Attachment.AttachmentRoot.Get() != nullptr)
	{
		DetachFromComponent();
	}

	AttachTo->Childs.emplace_back(this);
	Attachment.AttachmentRoot = AttachTo;
	Attachment.SocketName = socketName;

	UpdateComponentToWorld();
	OnAttachmentChanged();
}

void SceneComponent::DetachFromComponent()
{
	if (Attachment.AttachmentRoot.Get() == nullptr)
	{
		Log::Info(LogSceneComponent, TEXT("Component is already detached from any components. Abort."));
		return;
	}

	auto It = find(Childs.begin(), Childs.end(), this);
	if (It == Childs.end())
	{
		Log::Error(LogSceneComponent, TEXT("Cannot found this component from child component list of parent component."));
	}
	else
	{
		Childs.erase(It);
	}

	Attachment.AttachmentRoot = nullptr;
	Attachment.SocketName = TEXT("");

	UpdateComponentToWorld();
	OnAttachmentChanged();
}

Transform SceneComponent::GetRelativeTransform() const
{
	return RelativeTransform;
}

void SceneComponent::SetRelativeTransform(const Transform& value)
{
	RelativeTransform = value;
	UpdateWorldTransform();
}

Transform SceneComponent::GetComponentTransform() const
{
	return WorldTransform;
}

Vector3 SceneComponent::GetLocation()
{
	return RelativeTransform.Translation;
}

void SceneComponent::SetLocation(const Vector3& value)
{
	RelativeTransform.Translation = value;
	UpdateWorldTransform();
}

Vector3 SceneComponent::GetScale()
{
	return RelativeTransform.Scale;
}

void SceneComponent::SetScale(const Vector3& value)
{
	RelativeTransform.Scale = value;
	UpdateWorldTransform();
}

Quaternion SceneComponent::GetRotation()
{
	return RelativeTransform.Rotation;
}

void SceneComponent::SetRotation(const Quaternion& value)
{
	RelativeTransform.Rotation = value;
	UpdateWorldTransform();
}

void SceneComponent::OnAttachmentChanged()
{
	for (auto& Child : Childs)
	{
		Child->OnAttachmentChanged();
	}
}

void SceneComponent::UpdateWorldTransform()
{
	if (HasBegunPlay() && Mobility != EComponentMobility::Movable)
	{
		Log::Error(LogSceneComponent, TEXT("SceneComponent has been try move but it is not movable mobility."));
		return;
	}

	if (GetAttachParent() != nullptr)
	{
		WorldTransform = Transform::Multiply(GetAttachParent()->RelativeTransform, RelativeTransform);
	}
	else
	{
		WorldTransform = RelativeTransform;
	}

	UpdateChildTransforms();
}