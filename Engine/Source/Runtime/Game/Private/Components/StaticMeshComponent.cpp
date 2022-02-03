// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Components/StaticMeshComponent.h"
#include "LogGame.h"
#include "Scene/StaticMeshSceneProxy.h"
#include "Scene/StaticMeshRenderData.h"
#include "Materials/Material.h"

GENERATE_BODY(SStaticMeshComponent);

SStaticMeshComponent::SStaticMeshComponent() : Super()
{
}

PrimitiveSceneProxy* SStaticMeshComponent::CreateSceneProxy()
{
	if (_StaticMesh != nullptr)
	{
		return new StaticMeshSceneProxy(this, std::span(&_batch, 1));
	}
	else
	{
		return nullptr;
	}
}

void SStaticMeshComponent::SetStaticMesh(SStaticMesh* inNewMesh)
{
	//if (_StaticMesh != inNewMesh)
	//{
	//	_StaticMesh = inNewMesh;

	//	if (_StaticMesh != nullptr)
	//	{
	//		// StaticMeshComponent only render primary mesh batch.
	//		_batch = _StaticMesh->GetRenderData()->MeshBatches[0];

	//		if (_materials.size() < _batch.MaterialSlots.size())
	//		{
	//			_materials.resize(_batch.MaterialSlots.size());
	//		}

	//		for (size_t i = 0; i < _batch.MaterialSlots.size(); ++i)
	//		{
	//			_batch.MaterialSlots[i] = _materials[i];
	//		}
	//	}

	//	MarkRenderStateDirty();
	//}
}

void SStaticMeshComponent::SetMaterial(int32 index, SMaterial* material)
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

SMaterial* SStaticMeshComponent::GetMaterial(int32 index)
{
	if (index >= _materials.size())
	{
		SE_LOG(LogStaticMesh, Error, L"The index [{}] is not valid on this static mesh component.", index);
		return nullptr;
	}
	return _materials[index];
}

int32 SStaticMeshComponent::GetMaterialCount()
{
	return (int32)_materials.size();
}