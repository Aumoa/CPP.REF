// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneComponent.h"
#include "LogGame.h"

using namespace std;

using enum ELogVerbosity;

SceneComponent::SceneComponent() : Super()
{
}

void SceneComponent::UpdateChildTransforms()
{
	for (SceneComponent* child : _childComponents)
	{
		child->UpdateComponentToWorld();
	}
}

void SceneComponent::UpdateComponentToWorld()
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

Transform SceneComponent::GetSocketTransform(const wstring& socketName, EComponentTransformSpace space) const
{
	LogSystem::Log(LogSceneComponent, Error, L"SceneComponent::GetSocketName() called. SceneComponent have not any sockets. Use override this function and provide correct socket transform.");

	switch (space)
	{
	case EComponentTransformSpace::World:
		return GetComponentTransform();
	}

	return GetRelativeTransform();
}

bool SceneComponent::MoveComponent(const Vector3& inMoveDelta, const Quaternion& inNewRotation, EComponentTransformSpace inSpace)
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

void SceneComponent::AttachToComponent(SceneComponent* attachTo)
{
	AttachToSocket(attachTo, L"");
}

void SceneComponent::AttachToSocket(SceneComponent* attachTo, const wstring& socketName)
{
	if (attachTo == nullptr)
	{
		LogSystem::Log(LogSceneComponent, Warning, L"attachTo is nullptr. First argument of AttachToComponent function must not be nullptr. Abort.");
		return;
	}

	if (_attachment.AttachmentRoot == attachTo && _attachment.SocketName == socketName)
	{
		LogSystem::Log(LogSceneComponent, Verbose, L"Component is already attach to desired target. Abort.");
		return;
	}

	if (_attachment.AttachmentRoot != nullptr)
	{
		DetachFromComponent();
	}

	attachTo->_childComponents.emplace_back(this);
	_attachment.AttachmentRoot = attachTo;
	_attachment.SocketName = socketName;

	ForEachSceneComponents<SceneComponent>([attachTo](SceneComponent* sc)
	{
		if (sc->GetOwner() == nullptr)
		{
			sc->SetOwnerPrivate(attachTo->GetOwner());
		}
		return false;
	});
	UpdateComponentToWorld();
}

void SceneComponent::DetachFromComponent()
{
	if (_attachment.AttachmentRoot == nullptr)
	{
		LogSystem::Log(LogSceneComponent, Verbose, L"Component is already detached from any components. Abort.");
		return;
	}

	auto it = find(_childComponents.begin(), _childComponents.end(), this);
	if (it == _childComponents.end())
	{
		LogSystem::Log(LogSceneComponent, Error, L"Cannot found this component from child component list of parent component.");
	}
	else
	{
		_childComponents.erase(it);
	}

	_attachment.AttachmentRoot = nullptr;
	_attachment.SocketName = nullptr;

	UpdateComponentToWorld();
}

Transform SceneComponent::GetRelativeTransform() const
{
	return _transform;
}

void SceneComponent::SetRelativeTransform(const Transform& value)
{
	_transform = value;
	UpdateWorldTransform();
}

Transform SceneComponent::GetComponentTransform() const
{
	return _worldTransform;
}

Vector3 SceneComponent::GetLocation() const
{
	return _transform.Translation;
}

void SceneComponent::SetLocation(const Vector3& value)
{
	_transform.Translation = value;
	UpdateWorldTransform();
}

Vector3 SceneComponent::GetScale() const
{
	return _transform.Scale;
}

void SceneComponent::SetScale(const Vector3& value)
{
	_transform.Scale = value;
	UpdateWorldTransform();
}

Quaternion SceneComponent::GetRotation() const
{
	return _transform.Rotation;
}

void SceneComponent::SetRotation(const Quaternion& value)
{
	_transform.Rotation = value;
	UpdateWorldTransform();
}

EComponentMobility SceneComponent::GetMobility() const
{
	return _mobility;
}

void SceneComponent::SetMobility(EComponentMobility value)
{
	_mobility = value;
}

void SceneComponent::UpdateWorldTransform()
{
	if (HasBegunPlay() && _mobility != EComponentMobility::Movable)
	{
		LogSystem::Log(LogSceneComponent, Error, L"SceneComponent has been try move but it is not movable mobility.");
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