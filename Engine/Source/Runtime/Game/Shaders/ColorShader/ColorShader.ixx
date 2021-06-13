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
	vector<uint8> _vscode;
	vector<uint8> _pscode;

public:
	ColorShader(RHIDevice* device);

	virtual vector<RHIShaderParameterElement> GetShaderParameterDeclaration() const override;

protected:
	virtual span<uint8 const> CompileVS() override;
	virtual span<uint8 const> CompilePS() override;
};