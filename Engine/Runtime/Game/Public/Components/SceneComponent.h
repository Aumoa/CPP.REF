// Copyright 2020-2021 Aumoa.lib. All right reserved.

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
	UpdateProxy = RecreateProxy << 1,
	TransformUpdated = UpdateProxy << 1,
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
	vs_property_get(TRefPtr<String>, AttachSocketName);
	vs_property_get(const std::vector<SceneComponent*>&, ChildComponents);
	vs_property(Transform, RelativeTransform);
	vs_property_get(Transform, ComponentTransform);

	vs_property(Vector3, Location);
	vs_property(Vector3, Scale);
	vs_property(Quaternion, Rotation);

	vs_property_get_auto(Vector3, ComponentLocation, worldTransform.Translation);
	vs_property_get_auto(Vector3, ComponentScale, worldTransform.Scale);
	vs_property_get_auto(Quaternion, ComponentRotation, worldTransform.Rotation);
	vs_property_auto(EComponentMobility, Mobility, mobility);

private:
	void UpdateWorldTransform();
};

#include "SceneComponent.inl"