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

	Transform _transform;
	Transform _worldTransform;
	Transform _localToWorld;
	EComponentMobility _mobility = EComponentMobility::Movable;

	SceneAttachment _attachment;
	std::vector<SSceneComponent*> _childComponents;

public:
	SSceneComponent();

	virtual void UpdateChildTransforms();
	virtual void UpdateComponentToWorld();
	virtual Transform GetSocketTransform(const std::wstring& socketName, EComponentTransformSpace space = EComponentTransformSpace::World) const;
	virtual bool MoveComponent(const Vector3& inMoveDelta, const Quaternion& inNewRotation, EComponentTransformSpace inSpace = EComponentTransformSpace::World);

	void AttachToComponent(SSceneComponent* attachTo);
	void AttachToSocket(SSceneComponent* attachTo, const std::wstring& socketName);
	void DetachFromComponent();

	inline SSceneComponent* GetAttachParent() const { return _attachment.AttachmentRoot; }
	inline std::wstring GetAttachSocketName() const { return _attachment.SocketName; }
	inline const std::vector<SSceneComponent*>& GetChildComponents() const { return _childComponents; }
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

	template<std::derived_from<SSceneComponent> T>
	void ForEachSceneComponents(std::function<bool(const T*)> body) const
	{
		std::queue<const SSceneComponent*> hierarchy;
		hierarchy.emplace(this);

		while (!hierarchy.empty())
		{
			SSceneComponent* top = hierarchy.front();
			if constexpr (is_same_v<T, SSceneComponent>)
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

			std::vector<SSceneComponent*> childs = top->GetChildComponents();
			for (auto& child : childs)
			{
				hierarchy.emplace(child);
			}

			hierarchy.pop();
		}
	}

	template<std::derived_from<SSceneComponent> T>
	void ForEachSceneComponents(std::function<bool(T*)> body)
	{
		std::queue<SSceneComponent*> hierarchy;
		hierarchy.emplace(this);

		while (!hierarchy.empty())
		{
			SSceneComponent* top = hierarchy.front();
			if constexpr (std::is_same_v<T, SSceneComponent>)
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

			std::vector<SSceneComponent*> childs = top->GetChildComponents();
			for (auto& child : childs)
			{
				hierarchy.emplace(child);
			}

			hierarchy.pop();
		}
	}

	virtual void MarkRenderStateDirty() {}

private:
	void UpdateWorldTransform();
};