// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/SceneComponent.h"

#include "Logging/LogCategoryBase.h"
#include "Logging/LogMacros.h"

using namespace std;

LogCategoryBase SceneComponent::LogSceneComponent(ELogVerbosity::Verbose, L"LogSceneComponent");

SceneComponent::SceneAttachment::SceneAttachment()
	: AttachmentRoot(nullptr)
{

}

SceneComponent::SceneAttachment::~SceneAttachment()
{

}

void SceneComponent::SceneAttachment::Clear()
{
	AttachmentRoot = nullptr;
	SocketName = nullptr;
}

SceneComponent::SceneComponent() : Super()
	, transform(Transform::Identity)
	, localToWorld(Transform::Identity)
	, mobility(EComponentMobility::Static)
{

}

SceneComponent::~SceneComponent()
{

}

void SceneComponent::AttachToComponent(SceneComponent* attachTo)
{
	AttachToSocket(attachTo, nullptr);
}

void SceneComponent::AttachToSocket(SceneComponent* attachTo, TRefPtr<String> socketName)
{
	if (attachTo == nullptr)
	{
		SE_LOG(LogSceneComponent, Warning, L"{0} is nullptr. First argument of AttachToComponent function must not be nullptr. Abort.", nameof(attachTo));
		return;
	}

	if (componentAttachment.AttachmentRoot == attachTo && componentAttachment.SocketName == socketName)
	{
		SE_LOG(LogSceneComponent, Verbose, L"Component is already attach to desired target. Abort.");
		return;
	}

	if (componentAttachment.AttachmentRoot != nullptr)
	{
		DetachFromComponent();
	}

	attachTo->childComponents.emplace_back(this);
	componentAttachment.AttachmentRoot = attachTo;
	componentAttachment.SocketName = socketName;

	UpdateComponentToWorld();
}

void SceneComponent::DetachFromComponent()
{
	if (componentAttachment.AttachmentRoot == nullptr)
	{
		SE_LOG(LogSceneComponent, Verbose, L"Component is already detached from any components. Abort.");
		return;
	}

	auto it = find(childComponents.begin(), childComponents.end(), this);
	if (it == childComponents.end())
	{
		SE_LOG(LogSceneComponent, Error, L"Cannot found this component from child component list of parent component.");
	}
	else
	{
		childComponents.erase(it);
	}

	componentAttachment.AttachmentRoot = nullptr;
	componentAttachment.SocketName = nullptr;

	UpdateComponentToWorld();
}

void SceneComponent::UpdateChildTransforms()
{
	for (SceneComponent* child : childComponents)
	{
		child->UpdateComponentToWorld();
	}
}

void SceneComponent::UpdateComponentToWorld()
{
	if (AttachParent == nullptr)
	{
		localToWorld = Transform::Identity;
		return;
	}

	if (AttachSocketName.IsValid)
	{
		localToWorld = AttachParent->GetSocketTransform(AttachSocketName);
	}
	else
	{
		localToWorld = AttachParent->ComponentLocation;
	}

	UpdateChildTransforms();
}

Transform SceneComponent::GetSocketTransform(TRefPtr<String> socketName, EComponentTransformSpace space) const
{
	SE_LOG(LogSceneComponent, Error, L"SceneComponent::GetSocketName() called. SceneComponent have not any sockets. Use override this function and provide correct socket transform.");

	switch (space)
	{
	case EComponentTransformSpace::World:
		return ComponentTransform;
	}

	return RelativeTransform;
}

SceneComponent* SceneComponent::AttachParent_get() const
{
	return componentAttachment.AttachmentRoot;
}

TRefPtr<String> SceneComponent::AttachSocketName_get() const
{
	return componentAttachment.SocketName;
}

const vector<SceneComponent*>& SceneComponent::ChildComponents_get() const
{
	return childComponents;
}

Transform SceneComponent::RelativeTransform_get() const
{
	return transform;
}

void SceneComponent::RelativeTransform_set(const Transform& value)
{
	transform = value;
	UpdateChildTransforms();
}

Transform SceneComponent::ComponentTransform_get() const
{
	return Transform::Multiply(transform, localToWorld);
}

Vector3 SceneComponent::Location_get() const
{
	return transform.Translation;
}

void SceneComponent::Location_set(const Vector3& value)
{
	transform.Translation = value;
	UpdateChildTransforms();
}

Vector3 SceneComponent::Scale_get() const
{
	return transform.Scale;
}

void SceneComponent::Scale_set(const Vector3& value)
{
	transform.Scale = value;
	UpdateChildTransforms();
}

Quaternion SceneComponent::Rotation_get() const
{
	return transform.Rotation;
}

void SceneComponent::Rotation_set(const Quaternion& value)
{
	transform.Rotation = value;
	UpdateChildTransforms();
}

Vector3 SceneComponent::ComponentLocation_get() const
{
	return ComponentTransform.Translation;
}

Vector3 SceneComponent::ComponentScale_get() const
{
	return ComponentTransform.Scale;
}

Quaternion SceneComponent::ComponentRotation_get() const
{
	return ComponentTransform.Rotation;
}

EComponentMobility SceneComponent::Mobility_get() const
{
	return mobility;
}

void SceneComponent::Mobility_set(EComponentMobility value)
{
	mobility = value;
}