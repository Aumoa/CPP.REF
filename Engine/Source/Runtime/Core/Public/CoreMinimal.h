// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "LanguageSupportMacros.h"

#include "Platform/PlatformMacros.h"
#include "Platform/PlatformTypes.h"
// Do not include the PlatformCommon.h file.
// This file must be used with strict certification when using special features limited to the platform.
// Define the __ALLOW_PLATFORM_COMMON_H__ preprocessor in the source code you want to use.
// #include "Platform/PlatformCommon.h"

#include "Action.h"
#include "Func.h"
#include "StaticClass.h"
#include "IntegralTypes.h"
#include "StringComparison.h"
#include "StringSplitOptions.h"
#include "CharType.h"
#include "EaseFunction.h"
#include "Math.h"
#include "String.h"
#include "AssertionMacros.h"
#include "Radians.h"
#include "Degrees.h"
#include "TimeSpan.h"
#include "Path.h"
#include "RealTypes.h"
#include "DateTimeFormat.Localization.h"
#include "DateTimeFormat.Json.h"
#include "DateTimeFormat.File.h"
#include "DateTime.h"
#include "Delegate.h"
#include "MulticastDelegate.h"
#include "MulticastEvent.h"
#include "DelegateHandle.h"
#include "Exception.h"
#include "InvalidOperationException.h"
#include "NotSupportedException.h"
#include "AccessViolationException.h"
#include "ArgumentException.h"
#include "ArgumentNullException.h"
#include "ArgumentOutOfRangeException.h"
#include "InvalidCastException.h"
#include "NotImplementedException.h"
#include "NullReferenceException.h"
#include "StackOverflowException.h"
#include "SystemException.h"
#include "OperationCanceledException.h"
#include "ConstructorNotFoundException.h"
#include "Object.h"

#include "GC/GC.h"
#include "GC/RPtr.h"
#include "GC/PPtr.h"
#include "GC/GCPtr.Impl.h"

#include "Reflection/PropertyCollector.h"
#include "Reflection/PPtrGather.h"
#include "Reflection/ReflectionMacros.h"

#include "Diagnostics/Debug.h"

#include "Threading/CancellationToken.h"
#include "Threading/CancellationTokenSource.h"

#include "Threading/Tasks/TaskStatus.h"
#include "Threading/Tasks/suspend_and_destroy_if.h"
#include "Threading/Tasks/AwaiterBase.h"
#include "Threading/Tasks/co_push.h"
#include "Threading/Tasks/co_cancel.h"
#include "Threading/Tasks/Awaiter.h"
#include "Threading/Tasks/promise_type.h"
#include "Threading/Tasks/Task.h"
#include "Threading/Tasks/TaskCompletionSource.h"

#include "Localizational/Name.h"

// additional standard libraries.
#include <set>
#include <variant>