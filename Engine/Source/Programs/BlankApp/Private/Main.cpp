// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "CoreMinimal.h"
#include "NumericsCore.h"
#include "Numerics/MatrixInterface/AxisAlignedCube.h"
#include "Numerics/MatrixInterface/Line.h"
#include "Numerics/MatrixInterface/RotatedRect.h"
#include "Numerics/TransformInterface/Complex.h"
#include "Numerics/TransformInterface/Translate3D.h"
#include "Numerics/VectorInterface/Plane.h"
#include "Numerics/VectorInterface/Sphere.h"
#include "Numerics/VectorInterface/Range.h"
#include "Numerics/ObjectOrientedCube.h"
#include "Numerics/Ray.h"
#include "SampleClass.h"
#include "Type.h"

int main()
{
	PlatformProcess::SetupStacktraceSignals();
	Console::WriteLine(TEXT("BlankApp"));
	Vector3 V3;
	ASampleClass* Instance = NewObject<ASampleClass>();
	AType* Type = Instance->GetType();
	ASampleClass2* Instance2 = NewObject<ASampleClass2>();
	Type = Instance2->GetType();
	Console::WriteLine(Type->GetClassName());
	return 0;
}