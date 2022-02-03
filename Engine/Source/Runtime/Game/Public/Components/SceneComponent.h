// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent.h"
#include "GameEnums.h"

DECLARE_LOG_CATEGORY(GAME_API, LogSceneComponent);

/// <summary>
/// A SceneComponent has a transform and supports attachment, but has no rendering or collision capabilities.
/// </summary>
class GAME_API SSceneComponent : public SActorComponent
{
	GENERATED_BODY(SSceneComponent)

private:
	struct SceneAttachment
	{
		SSceneComponent* AttachmentRoot = nullptr;
		std::wstring SocketName;

		inline void Clear()
		{
			AttachmentRoot = nullptr;
			SocketName = L"";
		}
	};

	Transform _RelativeTransform;
	Transform _WorldTransform;

	SceneAttachment _Attachment;
	std::vector<SSceneComponent*> _Childs;

protected:
	EComponentMobility Mobility = EComponentMobility::Movable;

public:
	SSceneComponent();

	virtual void UpdateChildTransforms();
	virtual void UpdateComponentToWorld();
	virtual Transform GetSocketTransform(std::wstring_view InSocketName, EComponentTransformSpace InSpace = EComponentTransformSpace::World) const;
	virtual bool MoveComponent(const Vector3& InMoveDelta, const Quaternion& InNewRotation, EComponentTransformSpace InSpace = EComponentTransformSpace::World);

	void AttachToComponent(SSceneComponent* AttachTo);
	void AttachToSocket(SSceneComponent* AttachTo, const std::wstring& socketName);
	void DetachFromComponent();

	inline SSceneComponent* GetAttachParent() { return _Attachment.AttachmentRoot; }
	inline std::wstring GetAttachSocketName() { return _Attachment.SocketName; }
	inline const std::vector<SSceneComponent*>& GetChildComponents() { return _Childs; }
	Transform GetRelativeTransform() const;
	void SetRelativeTransform(const Transform& value);
	Transform GetComponentTransform() const;

	Vector3 GetLocation();
	void SetLocation(const Vector3& value);
	Vector3 GetScale();
	void SetScale(const Vector3& value);
	Quaternion GetRotation();
	void SetRotation(const Quaternion& value);

	inline Vector3 GetComponentLocation() { return _WorldTransform.Translation; }
	inline Vector3 GetComponentScale() { return _WorldTransform.Scale; }
	inline Quaternion GetComponentRotation() { return _WorldTransform.Rotation; }

	EComponentMobility GetMobility();
	void SetMobility(EComponentMobility value);

	virtual void MarkRenderStateDirty() {}

protected:
	virtual void OnAttachmentChanged();

private:
	void UpdateWorldTransform();
};