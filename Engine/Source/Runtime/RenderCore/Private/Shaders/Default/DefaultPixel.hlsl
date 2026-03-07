// Shader Type: pixel

#include "DefaultCommon.hlsli"

Pixel main(in Fragment fragment)
{
    Pixel p;
    p.Color = fragment.Color;
    return p;
}