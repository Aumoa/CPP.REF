// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:SceneComponent;

import std.core;
import :ActorComponent;
import :Transform;
import :ComponentMobility;
import :ComponentDirtyMask;
import :ComponentTransformSpace;

using namespace std;

/// <summary>
/// A SceneComponent has a transform and supports attachment, but has no rendering or collision capabilities.
/// </summary>
export class SceneComponent : public ActorComponent
{
public:
	using Super = ActorComponent;

private:
	struct SceneAttachment
	{
		SceneComponent* AttachmentRoot = nullptr;
		wstring SocketName;

		inline void Clear()
		{
			AttachmentRoot = nullptr;
			SocketName = L"";
		}
	};

	Transform _transform;
	Transform _worldTransform;
	Transform _localToWorld;
	EComponentMobility _mobility = EComponentMobility::Movable;
	EComponentDirtyMask _dirtyMark = EComponentDirtyMask::All;

	SceneAttachment _attachment;
	vector<SceneComponent*> _childComponents;

public:
	SceneComponent();
	~SceneComponent();

	virtual void UpdateChildTransforms();
	virtual void UpdateComponentToWorld();
	virtual Transform GetSocketTransform(const wstring& socketName, EComponentTransformSpace space = EComponentTransformSpace::World) const;
	virtual bool MoveComponent(const Vector3& inMoveDelta, const Quaternion& inNewRotation, EComponentTransformSpace inSpace = EComponentTransformSpace::World);

	void AttachToComponent(SceneComponent* attachTo);
	void AttachToSocket(SceneComponent* attachTo, const wstring& socketName);
	void DetachFromComponent();

	void SetMarkDirty(EComponentDirtyMask inSetMasks);
	bool HasAnyDirtyMark() const;
	bool HasDirtyMark(EComponentDirtyMask inMask) const;
	virtual void ResolveDirtyState();

	inline SceneComponent* GetAttachParent() const { return _attachment.AttachmentRoot; }
	inline wstring GetAttachSocketName() const { return _attachment.SocketName; }
	inline const vector<SceneComponent*>& GetChildComponents() const { return _childComponents; }
	Transform GetRelativeTransform() const;
	void SetRelativeTransform(const Transform& value);
	Transform GetComponentTransform() const;

	Vector3 GetLocation() const;
	void SetLocation(const Vector3& value);
	Vector3 GetScale() const;
	void SetScale(const Vector3& value);
	Quaternion GetRotation() const;
	void SetRotation(const Quaternion& value);

	inline Vector3 GetComponentLocation() const { return _worldTransform.Translation; }
	inline Vector3 GetComponentScale() const { return _worldTransform.Scale; }
	inline Quaternion GetComponentRotation() const { return _worldTransform.Rotation; }

	EComponentMobility GetMobility() const;
	void SetMobility(EComponentMobility value);

private:
	void UpdateWorldTransform();
};