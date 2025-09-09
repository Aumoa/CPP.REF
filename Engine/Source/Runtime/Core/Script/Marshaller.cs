using System.Runtime.InteropServices;

namespace Ayla;

public static unsafe class Marshaller
{
    public static nint StringArrayToNative(IReadOnlyList<string> values)
    {
        nint* array = (nint*)Marshal.AllocHGlobal(Marshal.SizeOf<nint>() * (values.Count + 1));
        array[0] = (nint)values.Count;
        for (int i = 0; i < values.Count; ++i)
        {
            array[i + 1] = Marshal.StringToHGlobalUni(values[i]);
        }

        return (nint)array;
    }

    public static nint ObjectArrayToNative<T>(IReadOnlyList<T> values) where T : Object
    {
        nint* array = (nint*)Marshal.AllocHGlobal(Marshal.SizeOf<nint>() * (values.Count + 1));
        array[0] = (nint)values.Count;
        for (int i = 0; i < values.Count; ++i)
        {
            array[i + 1] = values[i].InstanceId;
        }

        return (nint)array;
    }

    public static void ReleaseStringArray(nint array, int length)
    {
        nint* p = (nint*)array;
        for (int i = 0; i < length; ++i)
        {
            Marshal.FreeHGlobal(p[i + 1]);
        }
        Marshal.FreeHGlobal(array);
    }

    public static void ReleaseObjectArray(nint array)
    {
        Marshal.FreeHGlobal(array);
    }

    public static ObjectReferenceWrapper AsBinding(Object? aylaObject)
    {
        return aylaObject?.AsWrapper() ?? default;
    }
}
