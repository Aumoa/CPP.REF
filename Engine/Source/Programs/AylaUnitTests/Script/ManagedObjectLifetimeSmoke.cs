// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Runtime.CompilerServices;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla.Tests;

public static class ManagedObjectLifetimeSmoke
{
    private static readonly MethodInfo s_BeginWriteGCHandle = typeof(global::Ayla.Object)
        .GetMethod("BeginWriteGCHandle", BindingFlags.Static | BindingFlags.NonPublic)
        ?? throw new MissingMethodException(typeof(global::Ayla.Object).FullName, "BeginWriteGCHandle");

    private static readonly MethodInfo s_EndWriteGCHandleAndGetSerial = typeof(global::Ayla.Object)
        .GetMethod("EndWriteGCHandleAndGetSerial", BindingFlags.Static | BindingFlags.NonPublic)
        ?? throw new MissingMethodException(typeof(global::Ayla.Object).FullName, "EndWriteGCHandleAndGetSerial");

    private static ManagedLifetimeObject? s_Held;

    public static nint Hold(ObjectReferenceWrapper wrapper)
    {
        s_Held = wrapper.AsManaged<ManagedLifetimeObject>()
            ?? throw new InvalidOperationException("Failed to create managed lifetime wrapper.");
        return s_Held.NativePointer;
    }

    public static void DisposeHeld()
    {
        s_Held?.Dispose();
        s_Held = null;
        ForceFullCollection();
    }

    public static nint CreateUnheld(ObjectReferenceWrapper wrapper)
    {
        var nativePointer = CreateUnheldNoInlining(wrapper);
        ForceFullCollection();
        return nativePointer;
    }

    public static nint CreatePendingFinalizer(ObjectReferenceWrapper wrapper)
    {
        var nativePointer = CreateUnheldNoInlining(wrapper);
        GC.Collect();
        return nativePointer;
    }

    public static nint HoldAfterPendingFinalizer(ObjectReferenceWrapper wrapper)
    {
        GC.Collect();
        return Hold(wrapper);
    }

    public static nint ReplaceStaleWeakHandle(ObjectReferenceWrapper wrapper)
    {
        InstallCollectedWeakHandle(wrapper.Ptr);
        return Hold(wrapper);
    }

    public static unsafe void CreateHeldWrapperForNativeConsumption(ObjectReferenceWrapper wrapper, ObjectReferenceWrapper* output)
    {
        s_Held = wrapper.AsManaged<ManagedLifetimeObject>()
            ?? throw new InvalidOperationException("Failed to create managed lifetime wrapper.");
        *output = s_Held;
    }

    public static void ForceFullCollection()
    {
        GC.Collect();
        GC.WaitForPendingFinalizers();
        GC.Collect();
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static nint CreateUnheldNoInlining(ObjectReferenceWrapper wrapper)
    {
        var instance = wrapper.AsManaged<ManagedLifetimeObject>()
            ?? throw new InvalidOperationException("Failed to create managed lifetime wrapper.");
        return instance.NativePointer;
    }

    private static void InstallCollectedWeakHandle(nint nativePointer)
    {
        var staleHandle = CreateWeakHandle();
        ForceFullCollection();
        if (staleHandle.Target != null)
        {
            staleHandle.Free();
            throw new InvalidOperationException("Failed to collect stale weak handle target.");
        }

        var previousHandlePtr = BeginWriteGCHandle(nativePointer);
        try
        {
            if (previousHandlePtr != 0)
            {
                EndWriteGCHandleAndGetSerial(nativePointer, previousHandlePtr, false);
                throw new InvalidOperationException("Native object already has a managed handle.");
            }

            EndWriteGCHandleAndGetSerial(nativePointer, (nint)staleHandle, false);
        }
        catch
        {
            staleHandle.Free();
            throw;
        }
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static GCHandle CreateWeakHandle()
    {
        return GCHandle.Alloc(new object(), GCHandleType.Weak);
    }

    private static nint BeginWriteGCHandle(nint nativePointer)
    {
        return (nint)s_BeginWriteGCHandle.Invoke(null, [nativePointer])!;
    }

    private static ulong EndWriteGCHandleAndGetSerial(nint nativePointer, nint handle, bool releaseIntPtr)
    {
        return (ulong)s_EndWriteGCHandleAndGetSerial.Invoke(null, [nativePointer, handle, releaseIntPtr])!;
    }
}
