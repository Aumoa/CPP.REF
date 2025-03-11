// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/MatrixInterface/Matrix2x2.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Rect.h"

struct NSlateShaderPaintGeometry
{
    Matrix2x2 Transformation;
    Vector2 Translation;
    Vector2 LocalSize;
    Rect TextureCoordinate;
};