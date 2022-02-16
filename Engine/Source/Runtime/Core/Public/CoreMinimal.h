// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#ifdef _MSC_VER
#pragma warning(disable: 4250)
#pragma warning(disable: 4251)
#endif

#include "CoreMacros.h"
#include "PrimitiveTypes.h"
#include "Object.h"
#include "Casts.h"
#include "CoreAssert.h"
#include "ValueType.h"
#include "AbstractClass.h"
#include "NonCopyable.h"
#include "Guid.h"
#include "IDisposable.h"
#include "CoreConcepts.h"
#include "GC/GC.h"
#include "GC/SharedPtr.h"
#include "GC/WeakPtr.h"
#include "Reflection/Type.h"
#include "Reflection/Method.h"
#include "Reflection/Property.h"
#include "Reflection/SubclassOf.h"
#include "Reflection/Enum.h"
#include "Delegates/MulticastDelegate.h"
#include "Delegates/MulticastEvent.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/CycleCounterNamespace.h"
#include "Diagnostics/CycleCounterUnit.h"
#include "Mathematics/MathEx.h"
#include "Mathematics/Degrees.h"
#include "Mathematics/Radians.h"
#include "Numerics/Vector.h"
#include "Numerics/Matrix.h"
#include "Numerics/Matrix4x4.h"
#include "Numerics/Matrix2x2.h"
#include "Numerics/Matrix3x2.h"
#include "Numerics/AxisAlignedCube.h"
#include "Numerics/Color.h"
#include "Numerics/Frustum.h"
#include "Numerics/Line.h"
#include "Numerics/ObjectOrientedCube.h"
#include "Numerics/Plane.h"
#include "Numerics/Quaternion.h"
#include "Numerics/Complex.h"
#include "Numerics/Range.h"
#include "Numerics/Ray.h"
#include "Numerics/Sphere.h"
#include "Numerics/Rect.h"
#include "Numerics/Transform.h"
#include "Misc/StringUtils.h"
#include "Misc/DateTime.h"
#include "PlatformMisc/InputEnums.h"
#include "Threading/Task.h"