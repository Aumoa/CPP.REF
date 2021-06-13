// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game.Shaders:ColorVertexFactory;

import std.core;
import SC.Runtime.Core;
import SC.Runtime.RenderCore;

using namespace std;

export class ColorVertexFactory : public RHIVertexFactory
{
public:
	using Super = RHIVertexFactory;

#pragma pack(push, 4)
	struct MyVertex
	{
		Vector3 Pos;
		Vector3 Color;
	};
#pragma pack(pop)

public:
	ColorVertexFactory(RHIDevice* device);

	virtual RHIResource* CreateVertexBuffer(const RHIVertex* vertices, size_t count) const override;
	virtual uint32 GetVertexStride() const override { return sizeof(MyVertex); }
	virtual vector<RHIVertexElement> GetVertexDeclaration() const override;
};