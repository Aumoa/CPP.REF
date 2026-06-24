# Interop Exception Boundaries

This document records the exception propagation contract used by Ayla's C++/C# interop layer.
It is intended for both human maintainers and AI agents that need to modify RHT-generated calls or the foundational Core manual interop code.

## Goal

Interop calls must not let a C++ exception or a managed exception escape directly across the ABI boundary.
Instead, every exception-capable boundary reports failure with `NativeCallStatus.Exception`.
The receiving runtime then asks the matching interop store for the captured exception data and either:

- observes a wrapper exception while execution is still in the opposite runtime, or
- restores the original exception when control returns to the runtime where the exception was first thrown.

The intended model is:

| Origin | Intermediate Runtime Sees | Returned To Origin As |
| --- | --- | --- |
| C++ native exception | `Ayla.NativeException` in C# | Original C++ exception through `std::exception_ptr` |
| C# managed exception | `Ayla::ManagedException` in C++ | Original C# exception through `ExceptionDispatchInfo` |

## Boundary Contract

Use this contract for any interop call that can execute user, reflection, object lifetime, allocation, or managed activation logic.

1. Exported C++ functions that are called from C# should return `Ayla::NativeCallStatus` and be marked `noexcept`.
2. Those functions should wrap their body with `Ayla::NativeCallBoundary::Invoke`.
3. C# P/Invoke wrappers should call `Ayla.NativeCallBoundary.ThrowIfFailed(status)` before returning a result to normal managed code.
4. Managed functions called from C++ should return `Ayla.NativeCallStatus`.
5. Those managed functions should catch `Exception` and return `Ayla.ManagedCallBoundary.Capture(exception)`.
6. C++ callers should call `Ayla::ManagedCallBoundary::ThrowIfFailed(status)` before using output values.

Output values should be returned through pointer or `out` parameters when the function also needs to return `NativeCallStatus`.
This mirrors the generated RHT pattern and keeps the ABI shape explicit.

## RHT-Generated Calls

RHT-generated calls already follow this contract.
The relevant generator code is:

| Generator Area | Responsibility |
| --- | --- |
| `Engine/Source/Programs/AylaReflectionHeaderTool/RHT/CodeGen/CppClassGenerator.cs` | Emits exported native functions returning `::Ayla::NativeCallStatus` and wraps native bodies with `::Ayla::NativeCallBoundary::Invoke`. |
| `Engine/Source/Programs/AylaReflectionHeaderTool/RHT/CodeGen/CSharpClassGenerator.cs` | Emits managed invokers returning `global::Ayla.NativeCallStatus` and captures managed exceptions with `global::Ayla.ManagedCallBoundary.Capture`. |
| `Engine/Source/Programs/AylaReflectionHeaderTool/RHT/CodeGen/FunctionBodyGenerator.cs` | Emits `NativeCallBoundary.ThrowIfFailed(...)`, `ManagedCallBoundary.Succeed()`, and `ManagedCallBoundary::ThrowIfFailed(...)` at call sites. |

When adding a new generated call shape, prefer extending these same helpers rather than creating a separate exception channel.

## Core Manual Interop

Core contains foundational interop paths that RHT cannot generate because RHT depends on Core's object and scripting primitives.
These paths must still follow the same boundary contract.

### `Ayla::NativeCallBoundary`

Location: `Engine/Source/Runtime/Core/Public/Marshal/NativeCallBoundary.h`

`NativeCallBoundary::Invoke` is the C++ side of "native body called by managed code".
It catches all native exceptions and stores them through `NativeExceptionInterop`, returning `NativeCallStatus.Exception`.

Use it for:

- `PLATFORM_SHARED_EXPORT` functions callable from C#.
- Any native entry point where managed code would otherwise observe a raw native throw.

Do not use it for:

- Pure native-to-native calls.
- Functions that must deliberately crash or break into the debugger, such as assertion failure paths.

### `Ayla.NativeCallBoundary`

Location: `Engine/Source/Runtime/Core/Script/MarshalUtility/NativeCallBoundary.cs`

`NativeCallBoundary.ThrowIfFailed` is the C# side of "managed caller receives a native status".
It converts `NativeCallStatus.Exception` into either:

- a `NativeException` wrapper when the original exception came from native code, or
- the original managed exception when the native layer is returning a managed exception passport.

Use it in small managed wrapper methods around private P/Invoke declarations.
Avoid exposing raw `__Injected` methods as normal managed APIs when they can fail.

### `Ayla::ManagedCallBoundary`

Location: `Engine/Source/Runtime/Core/Public/Marshal/ManagedCallBoundary.h`

`ManagedCallBoundary::ThrowIfFailed` is the C++ side of "native caller receives a managed status".
It converts `NativeCallStatus.Exception` into either:

- a `ManagedException` wrapper when the original exception came from managed code, or
- the original native exception when managed code is returning a native exception passport.

Use it immediately after calling function pointers obtained from managed code.
Do not read output values from a managed callback until `ThrowIfFailed` has succeeded.

### `Ayla.ManagedCallBoundary`

Location: `Engine/Source/Runtime/Core/Script/MarshalUtility/ManagedCallBoundary.cs`

`ManagedCallBoundary.Succeed` and `ManagedCallBoundary.Capture` are the managed side of "managed body called by native code".
Managed callbacks exposed through function pointers should return `NativeCallStatus` and wrap their body in a `try`/`catch (Exception ex)` block.

This is required for `[UnmanagedCallersOnly]` methods because managed exceptions must not escape directly into native frames.

### `NativeExceptionInterop`

Locations:

- `Engine/Source/Runtime/Core/Public/Marshal/NativeExceptionInterop.h`
- `Engine/Source/Runtime/Core/Private/Marshal/NativeExceptionInterop.cpp`
- `Engine/Source/Runtime/Core/Script/MarshalUtility/NativeException.cs`

This stores native exception data for the current thread and, when possible, a token that references a captured `std::exception_ptr`.
The token lets native code restore the original native exception after it has crossed managed frames.

Maintain these invariants:

- `CaptureCurrentException` and `CaptureException` must be `noexcept`.
- `GetLastException` is thread-local state transfer; consumers should clear the last exception after reading.
- Captured native exception tokens must be released when they are not restored back into native code.
- If `NativeExceptionInfo.m_ManagedExceptionToken` is non-zero, C# should restore the managed exception instead of creating a `NativeException` wrapper.

### `ManagedExceptionInterop`

Locations:

- `Engine/Source/Runtime/Core/Public/Marshal/ManagedExceptionInterop.h`
- `Engine/Source/Runtime/Core/Private/Marshal/ManagedExceptionInterop.cpp`
- `Engine/Source/Runtime/Core/Script/MarshalUtility/ManagedExceptionInterop.cs`

This stores managed exception data and, when possible, a token that references a captured `ExceptionDispatchInfo`.
The token lets managed code restore the original managed exception after it has crossed native frames.

Maintain these invariants:

- `ManagedExceptionInterop.Capture` registers an `ExceptionDispatchInfo` only for real managed-origin exceptions.
- If the exception is a `NativeException`, capture should pass the native exception token back to native code instead of registering a new managed token.
- `ThrowCaptured` removes the token before rethrowing so the restored exception has single-consumer ownership.
- `CaptureException` on the native side must retain the managed token in `ManagedException` so a later native-to-managed return can restore the original managed exception.

### `CoreCLRFunctions`

Locations:

- `Engine/Source/Runtime/Core/Script/MarshalUtility/CoreCLRFunctions.cs`
- `Engine/Source/Runtime/Core/Private/Marshal/CoreCLRFunctions.h`

`CoreCLRFunctions` is a manually maintained table of managed callbacks used by Core native code.
It is foundational and cannot be generated by RHT.

The table follows the same rule as generated invokers:

- C# callback functions return `NativeCallStatus`.
- Results are written through pointer parameters.
- C++ callers immediately pass the status to `ManagedCallBoundary::ThrowIfFailed`.

Current callback responsibilities:

| Callback | Direction | Responsibility |
| --- | --- | --- |
| `m_AsHardHandle__Invoke` | C++ -> C# | Convert a weak or normal `GCHandle` to a normal handle while preserving the target. |
| `m_AsWeakHandle__Invoke` | C++ -> C# | Convert a normal `GCHandle` to a weak handle while preserving the target. |
| `m_CreateManagedInstancePtr__Invoke` | C++ -> C# | Create a managed wrapper instance for a script type and return an `ObjectReferenceWrapper` through an output pointer. |
| `m_FreeGCHandlePtr__Invoke` | C++ -> C# | Free a managed `GCHandle` owned by a wrapper handoff. |

When adding callbacks to this table, use `m_` field names on both sides and preserve sequential layout.

### `Object` Manual Exports

Locations:

- `Engine/Source/Runtime/Core/Public/Object.h`
- `Engine/Source/Runtime/Core/Private/Object.cpp`
- `Engine/Source/Runtime/Core/Script/Object.cs`
- `Engine/Source/Runtime/Core/Script/MarshalUtility/ObjectReferenceWrapper.cs`

`Object` is the main foundational class where manual C++/C# interop exists below RHT.
Its native exports are now status-returning boundary functions.
Managed code should call the checked wrapper methods instead of directly calling private `__Injected` P/Invokes.

Current wrapper responsibilities:

| Managed Wrapper | Native Export | Responsibility |
| --- | --- | --- |
| `BeginWriteGCHandle` | `Ayla__Object__BeginWriteGCHandle__Injected` | Lock the native object and read the current managed handle. |
| `EndWriteGCHandle` | `Ayla__Object__EndWriteGCHandle__Injected` | Replace the handle, update references, unlock, and delete the native object if ownership reaches zero. |
| `GetManagedType` | `Ayla__Object__GetManagedType__Injected` | Return the native/managed type pair for `Ayla.Object`. |
| `AsWrapper` | `Ayla__Object__AsWrapper__Injected` | Convert a native object pointer to an `ObjectReferenceWrapper`. |
| `GetManagedTypeFromPtr` | `Ayla__Object__GetManagedTypeFromPtr__Injected` | Return the native object's runtime managed type wrapper. |

The lock/unlock pairing in the GC handle write helpers is ownership-sensitive.
If a failure is possible between `BeginWriteGCHandle` and `EndWriteGCHandle`, managed code must ensure `EndWriteGCHandle` is called from a `catch` or `finally` path as appropriate.

## Paths Intentionally Not Converted

Not every Core P/Invoke should use the full exception boundary contract.

| Path | Reason |
| --- | --- |
| `ManagedStringWrapper.FreeIntRef__Injected` | Low-level release operation. It should remain simple and non-throwing; adding status handling would make disposal paths more fragile. |
| `NativeExceptionInterop` and `ManagedExceptionInterop` exported helpers | These are the boundary implementation itself. They must be simple enough to run while exception state is already being transferred. |
| `Debug.Ensure` and `Debug.Assert` exports | These are diagnostic/assertion paths that may deliberately break or become unreachable in check builds. Treating them as recoverable interop exceptions would change their semantics. |

If one of these paths starts executing user code or allocation-heavy logic, revisit this decision.

## Test Coverage

The expected passport behavior is covered by `AylaUnitTests`:

- `Engine/Source/Programs/AylaUnitTests/Private/Tests/ManagedInteropTest.cpp`
- `Engine/Source/Programs/AylaUnitTests/Script/ManagedInteropSmoke.cs`

The tests verify:

- a managed exception can cross a native frame, appear as `Ayla::ManagedException`, and restore as the original managed exception object;
- a native exception can cross a managed frame, appear as `Ayla.NativeException`, and restore as the original native exception type;
- a `CoreCLRFunctions` managed activation failure is reported to C++ as `Ayla::ManagedException` instead of escaping directly through an unmanaged callback;
- object lifetime tests still pass after Core manual interop was routed through the same boundary helpers.

Recommended verification command:

```powershell
dotnet Engine/Binaries/DotNET/AylaBuildTool.dll test --target AylaUnitTests --config Shipping --skip-shaders
```

## Extension Checklist

Use this checklist before adding or modifying interop code:

- Does this call cross the C++/C# ABI boundary?
- Can it allocate, reflect, activate objects, call user code, or throw?
- If C# calls C++, does the native function return `NativeCallStatus` and use `NativeCallBoundary::Invoke`?
- If C++ calls C#, does the managed function return `NativeCallStatus` and use `ManagedCallBoundary.Capture`?
- Are outputs passed through pointer or `out` parameters instead of replacing `NativeCallStatus`?
- Does the caller immediately call the matching `ThrowIfFailed` before reading output values?
- Is exception token ownership clear when the exception is observed in the opposite runtime and not returned to origin?
- Is there an AylaUnitTests case for the behavior, or is there a clear reason it is not practical?
