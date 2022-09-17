// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent.h"
#include "ComponentTransformSpace.h"
#include "ComponentMobility.h"
#include "SceneComponent.generated.h"

SCLASS()
class GAME_API SceneComponent : public ActorComponent
{
	GENERATED_BODY()

private:
	struct SceneAttachment
	{
		WeakPtr<SceneComponent> AttachmentRoot;
		String SocketName;

		inline void Clear() noexcept
		{
			AttachmentRoot.Reset();
			SocketName = TEXT("");
		}
	};

	Transform RelativeTransform;
	Transform WorldTransform;

	SceneAttachment Attachment;
	SPROPERTY()
	std::vector<SceneComponent*> Childs;

protected:
	SPROPERTY()
	EComponentMobility Mobility = EComponentMobility::Movable;

public:
	SceneComponent();

	virtual void UpdateChildTransforms();
	virtual void UpdateComponentToWorld();
	virtual Transform GetSocketTransform(const String& InSocketName, EComponentTransformSpace InSpace = EComponentTransformSpace::World) const;
	virtual bool MoveComponent(const Vector3& InMoveDelta, const Quaternion& InNewRotation, EComponentTransformSpace InSpace = EComponentTransformSpace::World);

	void AttachToComponent(SceneComponent* AttachTo);
	void AttachToSocket(SceneComponent* AttachTo, const String& SocketName);
	void DetachFromComponent();

	inline SceneComponent* GetAttachParent() { return Attachment.AttachmentRoot.Get(); }
	inline String GetAttachSocketName() { return Attachment.SocketName; }
	inline const std::vector<SceneComponent*>& GetChildComponents() { return Childs; }
	Transform GetRelativeTransform() const;
	void SetRelativeTransform(const Transform& value);
	Transform GetComponentTransform() const;

	Vector3 GetLocation();
	void SetLocation(const Vector3& value);
	Vector3 GetScale();
	void SetScale(const Vector3& value);
	Quaternion GetRotation();
	void SetRotation(const Quaternion& value);

	inline Vector3 GetComponentLocation() { return WorldTransform.Translation; }
	inline Vector3 GetComponentScale() { return WorldTransform.Scale; }
	inline Quaternion GetComponentRotation() { return WorldTransform.Rotation; }

	EComponentMobility GetMobility();
	void SetMobility(EComponentMobility value);

protected:
	virtual void OnAttachmentChanged();

private:
	void UpdateWorldTransform();
};