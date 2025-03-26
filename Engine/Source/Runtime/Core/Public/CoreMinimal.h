// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "LanguageSupportMacros.h"

#include "Linq/Linq.IsValidIndex.h"
#include "Linq/Linq.ToVector.h"
#include "Linq/Linq.IndexOf.h"
#include "Linq/Linq.Select.h"
#include "Linq/Linq.Where.h"
#include "Linq/Linq.Join.h"
#include "Linq/Linq.ToSet.h"
#include "Linq/Append.h"
#include "Linq/Concat.h"
#include "Linq/Any.h"
#include "Linq/Contains.h"
#include "Linq/Values.h"
#include "Linq/Keys.h"
#include "Linq/SelectMany.h"

#include "Platform/PlatformMacros.h"
#include "Platform/PlatformTypes.h"
#include "Platform/PlatformProcess.h"
#include "Platform/PlatformLocalization.h"
#include "Platform/PlatformAtomics.h"
#include "Platform/PlatformIO.h"
#include "Platform/ProcessStartInfo.h"
#include "Platform/Process.h"
#include "Platform/DynamicLibrary.h"

#include "Action.h"
#include "Func.h"
#include "VoidableRef.h"
#include "StaticClass.h"
#include "LanguageSupportMacros.h"
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
#include "ConsoleColor.h"
#include "TimeSpan.h"
#include "Guid.h"
#include "Path.h"
#include "RealTypes.h"
#include "DateTimeFormat.Localization.h"
#include "DateTimeFormat.Json.h"
#include "DateTimeFormat.File.h"
#include "DateTime.h"
#include "Delegate.h"
#include "MemoryAccessMode.h"
#include "VoidableOptional.h"
#include "Referencer.h"
#include "ScopedRemover.h"
#include "MulticastDelegate.h"
#include "MulticastEvent.h"
#include "DelegateHandle.h"
#include "TryFinally.h"
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
#include "VoidableVector.h"
#include "Console.h"
#include "Environment.h"
#include "CrcHash.h"
#include "Version.h"
#include "OperationCanceledException.h"
#include "ConstructorNotFoundException.h"

#include "Diagnostics/StackFrame.h"
#include "Diagnostics/StackTrace.h"
#include "Diagnostics/PerformanceTimer.h"
#include "Diagnostics/TickTimer.h"

#include "Threading/Spinlock.h"
#include "Threading/Thread.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Threading/ThreadPool.h"
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

#include "IO/TextWriter.h"
#include "IO/FileNotFoundException.h"
#include "IO/SearchOption.h"
#include "IO/Directory.h"
#include "IO/TextReader.h"
#include "IO/File.h"
#include "IO/FileAccessMode.h"
#include "IO/FileMode.h"
#include "IO/FileSharedMode.h"
#include "IO/SeekOrigin.h"
#include "IO/Stream.h"
#include "IO/StreamReader.h"
#include "IO/IOCompletionOverlapped.h"
#include "IO/FileStream.h"
#include "IO/StreamWriter.h"
#include "IO/FileSystemReference.h"
#include "IO/DirectoryReference.h"
#include "IO/FileReference.h"

#include "Localizational/Name.h"

// additional standard libraries.
#include <set>
#include <variant>