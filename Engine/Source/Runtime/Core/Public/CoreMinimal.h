// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "System/LanguageSupportMacros.h"

#include "Linq/Linq.IsValidIndex.h"
#include "Linq/Linq.ToVector.h"
#include "Linq/Linq.IndexOf.h"
#include "Linq/Linq.Select.h"
#include "Linq/Linq.Where.h"
#include "Linq/Linq.Join.h"
#include "Linq/Linq.Values.h"
#include "Linq/Linq.ToSet.h"
#include "Linq/Linq.Keys.h"
#include "Linq/Append.h"
#include "Linq/Concat.h"
#include "Linq/Any.h"
#include "Linq/Contains.h"

#include "Platform/PlatformMacros.h"
#include "Platform/PlatformTypes.h"
#include "Platform/PlatformProcess.h"
#include "Platform/PlatformLocalization.h"
#include "Platform/PlatformAtomics.h"
#include "Platform/PlatformIO.h"
#include "Platform/ToolChainInstallation.h"
#include "Platform/WindowsToolkitInstallation.h"
#include "Platform/ProcessStartInfo.h"
#include "Platform/Process.h"
#include "Platform/DynamicLibrary.h"

#include "System/Action.h"
#include "System/Func.h"
#include "System/VoidableRef.h"
#include "System/StaticClass.h"
#include "System/LanguageSupportMacros.h"
#include "System/IntegralTypes.h"
#include "System/StringComparison.h"
#include "System/StringSplitOptions.h"
#include "System/CharType.h"
#include "System/EaseFunction.h"
#include "System/Math.h"
#include "System/String.h"
#include "System/AssertionMacros.h"
#include "System/CommandLine.h"
#include "System/Radians.h"
#include "System/Degrees.h"
#include "System/ConsoleColor.h"
#include "System/TimeSpan.h"
#include "System/Guid.h"
#include "System/Path.h"
#include "System/RealTypes.h"
#include "System/DateTimeFormat.Localization.h"
#include "System/DateTimeFormat.Json.h"
#include "System/DateTimeFormat.File.h"
#include "System/DateTime.h"
#include "System/Delegate.h"
#include "System/MemoryAccessMode.h"
#include "System/VoidableOptional.h"
#include "System/Referencer.h"
#include "System/ScopedRemover.h"
#include "System/MulticastDelegate.h"
#include "System/MulticastEvent.h"
#include "System/DelegateHandle.h"
#include "System/Exception.h"
#include "System/InvalidOperationException.h"
#include "System/NotSupportedException.h"
#include "System/AccessViolationException.h"
#include "System/ArgumentException.h"
#include "System/ArgumentNullException.h"
#include "System/ArgumentOutOfRangeException.h"
#include "System/InvalidCastException.h"
#include "System/NotImplementedException.h"
#include "System/NullReferenceException.h"
#include "System/StackOverflowException.h"
#include "System/SystemException.h"
#include "System/VoidableVector.h"
#include "System/Console.h"
#include "System/Environment.h"
#include "System/CrcHash.h"
#include "System/Version.h"
#include "System/OperationCanceledException.h"

#include "Diagnostics/StackFrame.h"
#include "Diagnostics/StackTrace.h"
#include "Diagnostics/PerformanceTimer.h"
#include "Diagnostics/TickTimer.h"

#include "Threading/Spinlock.h"
#include "Threading/Thread.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Threading/ThreadPool.h"
#include "Threading/CancellationToken.h"

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