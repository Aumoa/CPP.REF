// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/SceneComponent.h"
#include "LogGame.h"

SSceneComponent::SSceneComponent() : Super()
{
}

void SSceneComponent::UpdateChildTransforms()
{
	for (SSceneComponent* child : _childComponents)
	{
		child->UpdateComponentToWorld();
	}
}

void SSceneComponent::UpdateComponentToWorld()
{
	if (GetAttachParent() == nullptr)
	{
		_localToWorld = Transform::GetIdentity();
		return;
	}

	if (GetAttachSocketName() != L"")
	{
		_localToWorld = GetAttachParent()->GetSocketTransform(GetAttachSocketName());
	}
	else
	{
		_localToWorld = GetAttachParent()->GetComponentTransform();
	}

	UpdateWorldTransform();
}

Transform SSceneComponent::GetSocketTransform(const std::wstring& socketName, EComponentTransformSpace space) const
{
	SE_LOG(LogSceneComponent, Error, L"SceneComponent::GetSocketName() called. SceneComponent have not any sockets. Use override this function and provide correct socket transform.");

	switch (space)
	{
	case EComponentTransformSpace::World:
		return GetComponentTransform();
	}

	return GetRelativeTransform();
}

bool SSceneComponent::MoveComponent(const Vector3& inMoveDelta, const Quaternion& inNewRotation, EComponentTransformSpace inSpace)
{
	Quaternion oldRotation = inSpace == EComponentTransformSpace::World ? GetComponentRotation() : GetRotation();
	if (inMoveDelta.NearlyEquals(Vector3::GetZero()) && oldRotation.NearlyEquals(inNewRotation))
	{
		// MoveDelta and NewRotation is nearly equals to previous component transform.
		// Skip moving and return state indicating that be not moved.
		return false;
	}

	Vector3 relativeLocation;
	Quaternion relativeRotation;

	if (inSpace == EComponentTransformSpace::World && GetAttachParent() != nullptr)
	{
		// Transform unit is only calculate from local space.
		// Therefore, will convert world space unit to local space unit and apply it.
		Vector3 newLocation = GetComponentLocation() + inMoveDelta;
		Quaternion newRotation = inNewRotation;

		auto worldTransform = Transform(newLocation, GetComponentScale(), newRotation);
		auto relativeTransform = Transform::GetRelativeTransform(worldTransform, GetAttachParent()->GetSocketTransform(GetAttachSocketName()));

		relativeLocation = relativeTransform.Translation;
		relativeRotation = relativeTransform.Rotation;
	}
	else
	{
		relativeLocation = GetLocation() + inMoveDelta;
		relativeRotation = inNewRotation;
	}

	_transform.Translation = relativeLocation;
	_transform.Rotation = relativeRotation;
	UpdateWorldTransform();

	return true;
}

void SSceneComponent::AttachToComponent(SSceneComponent* attachTo)
{
	AttachToSocket(attachTo, L"");
}

void SSceneComponent::AttachToSocket(SSceneComponent* attachTo, const std::wstring& socketName)
{
	if (attachTo == nullptr)
	{
		SE_LOG(LogSceneComponent, Warning, L"attachTo is nullptr. First argument of AttachToComponent function must not be nullptr. Abort.");
		return;
	}

	if (_attachment.AttachmentRoot == attachTo && _attachment.SocketName == socketName)
	{
		SE_LOG(LogSceneComponent, Verbose, L"Component is already attach to desired target. Abort.");
		return;
	}

	if (_attachment.AttachmentRoot != nullptr)
	{
		DetachFromComponent();
	}

	attachTo->_childComponents.emplace_back(this);
	_attachment.AttachmentRoot = attachTo;
	_attachment.SocketName = socketName;

	ForEachSceneComponents<SSceneComponent>([attachTo](SSceneComponent* sc)
	{
		if (sc->GetOwner() == nullptr)
		{
			sc->SetOwnerPrivate(attachTo->GetOwner());
		}
		return false;
	});
	UpdateComponentToWorld();
}

void SSceneComponent::DetachFromComponent()
{
	if (_attachment.AttachmentRoot == nullptr)
	{
		SE_LOG(LogSceneComponent, Verbose, L"Component is already detached from any components. Abort.");
		return;
	}

	auto it = find(_childComponents.begin(), _childComponents.end(), this);
	if (it == _childComponents.end())
	{
		SE_LOG(LogSceneComponent, Error, L"Cannot found this component from child component list of parent component.");
	}
	else
	{
		_childComponents.erase(it);
	}

	_attachment.AttachmentRoot = nullptr;
	_attachment.SocketName = L"";

	UpdateComponentToWorld();
}

Transform SSceneComponent::GetRelativeTransform() const
{
	return _transform;
}

void SSceneComponent::SetRelativeTransform(const Transform& value)
{
	_transform = value;
	UpdateWorldTransform();
}

Transform SSceneComponent::GetComponentTransform() const
{
	return _worldTransform;
}

Vector3 SSceneComponent::GetLocation()
{
	return _transform.Translation;
}

void SSceneComponent::SetLocation(const Vector3& value)
{
	_transform.Translation = value;
	UpdateWorldTransform();
}

Vector3 SSceneComponent::GetScale()
{
	return _transform.Scale;
}

void SSceneComponent::SetScale(const Vector3& value)
{
	_transform.Scale = value;
	UpdateWorldTransform();
}

Quaternion SSceneComponent::GetRotation()
{
	return _transform.Rotation;
}

void SSceneComponent::SetRotation(const Quaternion& value)
{
	_transform.Rotation = value;
	UpdateWorldTransform();
}

EComponentMobility SSceneComponent::GetMobility()
{
	return _mobility;
}

void SSceneComponent::SetMobility(EComponentMobility value)
{
	_mobility = value;
}

void SSceneComponent::UpdateWorldTransform()
{
	if (HasBegunPlay() && _mobility != EComponentMobility::Movable)
	{
		SE_LOG(LogSceneComponent, Error, L"SceneComponent has been try move but it is not movable mobility.");
		return;
	}

	if (GetAttachParent() != nullptr)
	{
		_worldTransform = Transform::Multiply(_transform, _localToWorld);
	}
	else
	{
		_worldTransform = _transform;
	}

	UpdateChildTransforms();
}