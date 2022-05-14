// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Components/SceneComponent.h"

DEFINE_LOG_CATEGORY(LogSceneComponent);

using namespace ::libty;

SSceneComponent::SSceneComponent() : Super()
{
}

void SSceneComponent::UpdateChildTransforms()
{
	for (SSceneComponent* child : _Childs)
	{
		child->UpdateComponentToWorld();
	}
}

void SSceneComponent::UpdateComponentToWorld()
{
	if (GetAttachParent() == nullptr)
	{
		_RelativeTransform = Transform::Identity();
		return;
	}

	if (GetAttachSocketName() != TEXT(""))
	{
		_RelativeTransform = GetAttachParent()->GetSocketTransform(GetAttachSocketName());
	}
	else
	{
		_RelativeTransform = GetAttachParent()->GetComponentTransform();
	}

	UpdateWorldTransform();
}

Transform SSceneComponent::GetSocketTransform(String InSocketName, EComponentTransformSpace InSpace) const
{
	SE_LOG(LogSceneComponent, Error, TEXT("SceneComponent::GetSocketName() called. SceneComponent have not any sockets. Use override this function and provide correct socket transform."));

	switch (InSpace)
	{
	case EComponentTransformSpace::World:
		return GetComponentTransform();
	default:
		return GetRelativeTransform();
	}
}

bool SSceneComponent::MoveComponent(const Vector3& InMoveDelta, const Quaternion& InNewRotation, EComponentTransformSpace InSpace)
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

	_RelativeTransform.Translation = RelativeLocation;
	_RelativeTransform.Rotation = RelativeRotation;
	UpdateWorldTransform();

	return true;
}

void SSceneComponent::AttachToComponent(SSceneComponent* AttachTo)
{
	AttachToSocket(AttachTo, TEXT(""));
}

void SSceneComponent::AttachToSocket(SSceneComponent* AttachTo, const String& socketName)
{
	if (AttachTo == nullptr)
	{
		SE_LOG(LogSceneComponent, Warning, TEXT("AttachTo is nullptr. First argument of AttachToComponent function must not be nullptr. Abort."));
		return;
	}

	if (_Attachment.AttachmentRoot == AttachTo && _Attachment.SocketName == socketName)
	{
		SE_LOG(LogSceneComponent, Verbose, TEXT("Component is already attach to desired target. Abort."));
		return;
	}

	if (_Attachment.AttachmentRoot != nullptr)
	{
		DetachFromComponent();
	}

	AttachTo->_Childs.emplace_back(this);
	_Attachment.AttachmentRoot = AttachTo;
	_Attachment.SocketName = socketName;

	UpdateComponentToWorld();
	OnAttachmentChanged();
}

void SSceneComponent::DetachFromComponent()
{
	if (_Attachment.AttachmentRoot == nullptr)
	{
		SE_LOG(LogSceneComponent, Verbose, TEXT("Component is already detached from any components. Abort."));
		return;
	}

	auto It = find(_Childs.begin(), _Childs.end(), this);
	if (It == _Childs.end())
	{
		SE_LOG(LogSceneComponent, Error, TEXT("Cannot found this component from child component list of parent component."));
	}
	else
	{
		_Childs.erase(It);
	}

	_Attachment.AttachmentRoot = nullptr;
	_Attachment.SocketName = TEXT("");

	UpdateComponentToWorld();
	OnAttachmentChanged();
}

Transform SSceneComponent::GetRelativeTransform() const
{
	return _RelativeTransform;
}

void SSceneComponent::SetRelativeTransform(const Transform& value)
{
	_RelativeTransform = value;
	UpdateWorldTransform();
}

Transform SSceneComponent::GetComponentTransform() const
{
	return _WorldTransform;
}

Vector3 SSceneComponent::GetLocation()
{
	return _RelativeTransform.Translation;
}

void SSceneComponent::SetLocation(const Vector3& value)
{
	_RelativeTransform.Translation = value;
	UpdateWorldTransform();
}

Vector3 SSceneComponent::GetScale()
{
	return _RelativeTransform.Scale;
}

void SSceneComponent::SetScale(const Vector3& value)
{
	_RelativeTransform.Scale = value;
	UpdateWorldTransform();
}

Quaternion SSceneComponent::GetRotation()
{
	return _RelativeTransform.Rotation;
}

void SSceneComponent::SetRotation(const Quaternion& value)
{
	_RelativeTransform.Rotation = value;
	UpdateWorldTransform();
}

void SSceneComponent::OnAttachmentChanged()
{
	for (auto& Child : _Childs)
	{
		Child->OnAttachmentChanged();
	}
}

void SSceneComponent::UpdateWorldTransform()
{
	if (HasBegunPlay() && Mobility != EComponentMobility::Movable)
	{
		SE_LOG(LogSceneComponent, Error, TEXT("SceneComponent has been try move but it is not movable mobility."));
		return;
	}

	if (GetAttachParent() != nullptr)
	{
		_WorldTransform = Transform::Multiply(_RelativeTransform, _RelativeTransform);
	}
	else
	{
		_WorldTransform = _RelativeTransform;
	}

	UpdateChildTransforms();
}