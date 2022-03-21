namespace ColorShaderHLSL
{
	const char PixelShader[] = R"(
#include "CommonHLSL/CommonTypes.hlsli"

void PSMain(in Fragment pIn, out Pixel pOut)
{
	pOut.Color = pIn.Color;
}
)";
}