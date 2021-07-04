// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMeshComponent.h"
#include "LogGame.h"
#include "Scene/StaticMeshSceneProxy.h"
#include "Scene/StaticMeshRenderData.h"
#include "Assets/StaticMesh.h"

using namespace std;

using enum ELogVerbosity;

StaticMeshComponent::StaticMeshComponent() : Super()
{
}

PrimitiveSceneProxy* StaticMeshComponent::CreateSceneProxy()
{
	if (_StaticMesh != nullptr)
	{
		return CreateSubobject<StaticMeshSceneProxy>(this, vector{ _batch });
	}
	else
	{
		return nullptr;
	}
}

void StaticMeshComponent::SetStaticMesh(StaticMesh* inNewMesh)
{
	if (_StaticMesh != inNewMesh)
	{
		_StaticMesh = inNewMesh;

		if (_StaticMesh != nullptr)
		{
			// StaticMeshComponent only render primary mesh batch.
			_batch = _StaticMesh->GetRenderData()->MeshBatches[0];

			if (_materials.size() < _batch.MaterialSlots.size())
			{
				_materials.resize(_batch.MaterialSlots.size());
			}

			for (size_t i = 0; i < _batch.MaterialSlots.size(); ++i)
			{
				_batch.MaterialSlots[i] = _materials[i];
			}
		}

		MarkRenderStateDirty();
	}
}

void StaticMeshComponent::SetMaterial(int32 index, Material* material)
{
	if (index >= _materials.size())
	{
		_materials.resize((size_t)index + 1);
	}

	if (_materials[index] != material)
	{
		_materials[index] = material;
	}

	if (_StaticMesh && index < _batch.MaterialSlots.size())
	{
		if (_batch.MaterialSlots[index] != material)
		{
			_batch.MaterialSlots[index] = material;
			MarkRenderStateDirty();
		}
	}
}

Material* StaticMeshComponent::GetMaterial(int32 index) const
{
	if (index >= _materials.size())
	{
		LogSystem::Log(LogStaticMesh, Error, L"The index [{}] is not valid on this static mesh component.", index);
		return nullptr;
	}
	return _materials[index];
}

int32 StaticMeshComponent::GetMaterialCount() const
{
	return (int32)_materials.size();
}