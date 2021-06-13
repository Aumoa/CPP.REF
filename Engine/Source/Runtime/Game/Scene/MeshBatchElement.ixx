// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:MeshBatchElement;

import SC.Runtime.Core;
import SC.Runtime.RenderCore;

export struct MeshBatchElement
{
	uint64 VertexBufferLocation = 0;
	uint64 IndexBufferLocation = 0;

	uint32 IndexCount = 0;
	uint32 InstanceCount = 0;
	int32 StartIndexLocation = 0;
	int32 BaseVertexLocation = 0;
	uint32 StartInstanceLocation = 0;
	uint32 VertexCount = 0;
};