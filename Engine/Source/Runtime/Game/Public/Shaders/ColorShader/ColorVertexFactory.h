// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIVertexFactory.h"
#include <vector>

class GAME_API ColorVertexFactory : public RHIVertexFactory
{
	CLASS_BODY(ColorVertexFactory)

public:
#pragma pack(push, 4)
	struct MyVertex
	{
		Vector3 Pos;
		Vector3 Color;
		Vector3 Normal;
	};
#pragma pack(pop)

public:
	ColorVertexFactory(RHIDevice* device);

	virtual RHIResource* CreateVertexBuffer(const RHIVertex* vertices, size_t count) const override;
	virtual uint32 GetVertexStride() const override { return sizeof(MyVertex); }
	virtual std::vector<RHIVertexElement> GetVertexDeclaration() const override;
};