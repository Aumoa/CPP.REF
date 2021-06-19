// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent.h"
#include <vector>
#include <queue>
#include "GameEnums.h"
#include "GameStructures.h"

/// <summary>
/// A SceneComponent has a transform and supports attachment, but has no rendering or collision capabilities.
/// </summary>
class SceneComponent : public ActorComponent
{
public:
	using Super = ActorComponent;

private:
	struct SceneAttachment
	{
		SceneComponent* AttachmentRoot = nullptr;
		std::wstring SocketName;

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
	std::vector<SceneComponent*> _childComponents;

public:
	SceneComponent();

	virtual void UpdateChildTransforms();
	virtual void UpdateComponentToWorld();
	virtual Transform GetSocketTransform(const std::wstring& socketName, EComponentTransformSpace space = EComponentTransformSpace::World) const;
	virtual bool MoveComponent(const Vector3& inMoveDelta, const Quaternion& inNewRotation, EComponentTransformSpace inSpace = EComponentTransformSpace::World);

	void AttachToComponent(SceneComponent* attachTo);
	void AttachToSocket(SceneComponent* attachTo, const std::wstring& socketName);
	void DetachFromComponent();

	void SetMarkDirty(EComponentDirtyMask inSetMasks);
	bool HasAnyDirtyMark() const;
	bool HasDirtyMark(EComponentDirtyMask inMask) const;
	virtual void ResolveDirtyState();

	inline SceneComponent* GetAttachParent() const { return _attachment.AttachmentRoot; }
	inline std::wstring GetAttachSocketName() const { return _attachment.SocketName; }
	inline const std::vector<SceneComponent*>& GetChildComponents() const { return _childComponents; }
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

	template<std::derived_from<SceneComponent> T>
	void ForEachSceneComponents(std::function<bool(const T*)> body) const
	{
		std::queue<const SceneComponent*> hierarchy;
		hierarchy.emplace(this);

		while (!hierarchy.empty())
		{
			SceneComponent* top = hierarchy.front();
			if constexpr (is_same_v<T, SceneComponent>)
			{
				if (body(top))
				{
					break;
				}
			}
			else
			{
				if (auto* ptr = dynamic_cast<T*>(top); ptr != nullptr)
				{
					if (body(ptr))
					{
						break;
					}
				}
			}

			std::vector<SceneComponent*> childs = top->GetChildComponents();
			for (auto& child : childs)
			{
				hierarchy.emplace(child);
			}

			hierarchy.pop();
		}
	}

	template<std::derived_from<SceneComponent> T>
	void ForEachSceneComponents(std::function<bool(T*)> body)
	{
		std::queue<SceneComponent*> hierarchy;
		hierarchy.emplace(this);

		while (!hierarchy.empty())
		{
			SceneComponent* top = hierarchy.front();
			if constexpr (is_same_v<T, SceneComponent>)
			{
				if (body(top))
				{
					break;
				}
			}
			else
			{
				if (auto* ptr = dynamic_cast<T*>(top); ptr != nullptr)
				{
					if (body(ptr))
					{
						break;
					}
				}
			}

			std::vector<SceneComponent*> childs = top->GetChildComponents();
			for (auto& child : childs)
			{
				hierarchy.emplace(child);
			}

			hierarchy.pop();
		}
	}

private:
	void UpdateWorldTransform();
};