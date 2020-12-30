// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/MeshBatch.h"

MeshBatch::MeshBatch() : Super()
	, drawCommand(nullptr)
{
	
}

MeshBatch::~MeshBatch()
{

}

RHIMeshDrawCommand* MeshBatch::GetDrawCommand() const
{
	return drawCommand;
}