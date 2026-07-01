// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Runtime.CompilerServices;

namespace Ayla.Tests;

public static class ManagedObjectLifetimeSmoke
{
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
}
