// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ActorComponent.h"

#include "Transform.h"
#include "ComponentTransformSpace.h"
#include "ComponentMobility.h"

struct LogCategoryBase;

enum class EComponentDirtyMask : uint32
{
	None = 0,
	RecreateProxy = 1,
	TransformUpdated = RecreateProxy << 1,
	Last = TransformUpdated,
	All = 0xFFFFFFFF,
};

class GAME_API SceneComponent : public ActorComponent
{
public:
	using Super = ActorComponent;
	using This = SceneComponent;

private:
	struct SceneAttachment
	{
		SceneComponent* AttachmentRoot;
		TRefPtr<String> SocketName;

		SceneAttachment();
		~SceneAttachment();

		void Clear();
	};

private:
	Transform transform;
	Transform worldTransform;
	Transform localToWorld;
	EComponentMobility mobility;
	EComponentDirtyMask dirtyMark;

	SceneAttachment componentAttachment;
	std::vector<SceneComponent*> childComponents;

public:
	SceneComponent();
	~SceneComponent();
	
	virtual void UpdateChildTransforms();
	virtual void UpdateComponentToWorld();
	virtual Transform GetSocketTransform(TRefPtr<String> socketName, EComponentTransformSpace space = EComponentTransformSpace::World) const;
	virtual bool MoveComponent(const Vector3& inMoveDelta, const Quaternion& inNewRotation, EComponentTransformSpace inSpace = EComponentTransformSpace::World);

	void AttachToComponent(SceneComponent* attachTo);
	void AttachToSocket(SceneComponent* attachTo, TRefPtr<String> socketName);
	void DetachFromComponent();

	void SetMarkDirty(EComponentDirtyMask inSetMasks);
	bool HasAnyDirtyMark() const;
	bool HasDirtyMark(EComponentDirtyMask inMask) const;
	virtual void ResolveDirtyState();

	vs_property_get(SceneComponent*, AttachParent);
	SceneComponent* AttachParent_get() const;
	vs_property_get(TRefPtr<String>, AttachSocketName);
	TRefPtr<String> AttachSocketName_get() const;
	vs_property_get(const std::vector<SceneComponent*>&, ChildComponents);
	const std::vector<SceneComponent*>& ChildComponents_get() const;
	vs_property(Transform, RelativeTransform);
	Transform RelativeTransform_get() const;
	void RelativeTransform_set(const Transform& value);
	vs_property_get(Transform, ComponentTransform);
	Transform ComponentTransform_get() const;

	vs_property(Vector3, Location);
	Vector3 Location_get() const;
	void Location_set(const Vector3& value);
	vs_property(Vector3, Scale);
	Vector3 Scale_get() const;
	void Scale_set(const Vector3& value);
	vs_property(Quaternion, Rotation);
	Quaternion Rotation_get() const;
	void Rotation_set(const Quaternion& value);

	vs_property_get_auto(Vector3, ComponentLocation, worldTransform.Translation);
	vs_property_get_auto(Vector3, ComponentScale, worldTransform.Scale);
	vs_property_get_auto(Quaternion, ComponentRotation, worldTransform.Rotation);
	vs_property_auto(EComponentMobility, Mobility, mobility);

private:
	void UpdateWorldTransform();
};

#include "SceneComponent.inl"