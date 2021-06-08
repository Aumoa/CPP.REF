// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game.Shaders:ColorShader;

import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import std.core;

using namespace std;

/// <summary>
/// Represents default shader bytecode.
/// </summary>
export class ColorShader : public RHIShader
{
public:
	using Super = RHIShader;

private:
#pragma pack(push, 4)
	struct MyVertex
	{
		Vector3 Pos;
		Vector3 Color;
	};
#pragma pack(pop)

private:
	vector<uint8> _vscode;
	vector<uint8> _pscode;

public:
	ColorShader(RHIDevice* device);

	virtual RHIResource* CreateVertexBuffer(const RHIVertex* vertices, size_t count) const override;
	virtual uint32 GetVertexStride() const override { return sizeof(MyVertex); }

protected:
	virtual span<uint8 const> CompileVS() override;
	virtual span<uint8 const> CompilePS() override;
	virtual vector<RHIVertexElement> GetVertexDeclaration() const override;
};