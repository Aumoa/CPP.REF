// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHIMeshDrawCommand.h"

RHIMeshDrawCommand::RHIMeshDrawCommand()
	: Topology(ERHIPrimitiveTopology::TriangleList)

	, VertexBufferVirtualAddress(0)
	, IndexBufferVirtualAddress(0)
	
	, VertexCount(0)
	, IndexCount(0)
	, VertexStride(0)

	, MaterialIndex(0)
{

}