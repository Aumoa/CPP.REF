// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VertexFactory.h"

class GAME_API SColorVertexFactory : public SVertexFactory
{
	GENERATED_BODY(SColorVertexFactory)

public:
#pragma pack(push, 4)
	struct MyVertex
	{
		Vector3 Pos;
		Vector3 Color;
		Vector3 Normal;
	};
#pragma pack(pop)

private:
	IRHIDevice* _device = nullptr;

public:
	SColorVertexFactory(IRHIDevice* device);

	virtual IRHIBuffer* CreateVertexBuffer(const RHIVertex* vertices, size_t count) const override;
	virtual uint32 GetVertexStride() const override { return sizeof(MyVertex); }
	virtual std::vector<RHIVertexElement> GetVertexDeclaration() const override;
};