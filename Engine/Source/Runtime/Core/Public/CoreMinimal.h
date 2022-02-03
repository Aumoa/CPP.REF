// Copyright 2020-2022 Aumoa.lib. All right reserved.

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
#include "CoreNumerics.h"
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
#include "Misc/StringUtils.h"
#include "Misc/DateTime.h"
#include "PlatformMisc/InputEnums.h"
#include "Threading/Task.h"