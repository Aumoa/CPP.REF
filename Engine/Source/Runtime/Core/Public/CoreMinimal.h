// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

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
#include "NonCopyable.h"
#include "IntegralTypes.h"
#include "StringComparison.h"
#include "StringSplitOptions.h"
#include "CharType.h"
#include "EaseFunction.h"
#include "Math_.h"
#include "String_.h"
#include "StringBuilder.h"
#include "AssertionMacros.h"
#include "Radians.h"
#include "Degrees.h"
#include "Math.Impl.h"
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
#include "ObjectDisposedException.h"
#include "MemberAccessException.h"
#include "ManagedException.h"
#include "TaskCanceledException.h"
#include "AggregateException.h"
#include "FormatException.h"
#include "Object.h"
#include "Type.h"
#include "try_finally.h"

#include "Reflection/PropertyCollector.h"
#include "Reflection/ReflectionMacros.h"
#include "Reflection/TypeRegister.h"
#include "Reflection/reflexpr.h"
#include "Reflection/TransferFunction.h"
#include "Reflection/ConstructorInfo.h"
#include "Reflection/ReflectionMacros.h"

#include "Marshal/ObjectReferenceWrappers.h"
#include "Marshal/ManagedStringWrapper.h"
#include "Marshal/ManagedArrayWrapper.h"
#include "Marshal/ManagedTypeWrapper.h"
#include "Marshal/ManagedTaskWrapper.h"
#include "Marshal/NativeExceptionInterop.h"
#include "Marshal/ManagedExceptionInterop.h"
#include "Marshal/NativeCallBoundary.h"
#include "Marshal/ManagedCallBoundary.h"

#include "Diagnostics/Debug.h"
#include "Diagnostics/LogMacros.h"

#include "Threading/SynchronizationContext.h"
#include "Threading/Tasks/TaskStatus.h"
#include "Threading/Tasks/Task.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include "Threading/Tasks/TaskScheduler.h"
#include "Threading/Tasks/TaskFactory.h"

#include "Localizational/Name.h"

#include "ScriptingBackend/ScriptingBackend.h"
#include "ScriptingBackend/CoreCLR/CoreCLRScriptingBackend.h"

// additional standard libraries.
#include <set>
#include <variant>

namespace Game
{
	using namespace ::Ayla;
}
