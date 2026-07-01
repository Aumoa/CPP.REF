using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public readonly struct ManagedArrayWrapper : IDisposable
{
    public readonly nint Data;
    public readonly int Length;

    public ManagedArrayWrapper(nint data, int length)
    {
        Data = data;
        Length = length;
    }

    public unsafe string[] AsStringArray()
    {
        var input = (byte*)Data;

        int count = BitConverter.ToInt32(new ReadOnlySpan<byte>(input, sizeof(int)));
        int seekpos = sizeof(int);

        string[] output = new string[count];
        for (int i = 0; i < count; ++i)
        {
            int strLength = BitConverter.ToInt32(new ReadOnlySpan<byte>(input + seekpos, sizeof(int)));
            seekpos += sizeof(int);

            output[i] = new string((char*)(input + seekpos), 0, strLength);
            seekpos += sizeof(char) * strLength;
        }

        return output;
    }

    public unsafe T[] AsObjectArray<T>() where T : Object
    {
        var input = (NativeObjectReferenceWrapper*)Data;
        T[] output = new T[Length];
        for (int i = 0; i < Length; ++i)
        {
            output[i] = input[i].AsManaged<T>()!;
        }
        return output;
    }

    public unsafe T[] AsArray<T>() where T : unmanaged
    {
        var input = (T*)Data;
        T[] output = new T[Length];
        for (int i = 0; i < Length; ++i)
        {
            output[i] = input[i];
        }
        return output;
    }

    public void Dispose()
    {
        // Dispose should only be called when invoked from C#.
        Marshal.FreeHGlobal(Data);
    }

    public static unsafe ManagedArrayWrapper FromArray<T>(T[] array) where T : unmanaged
    {
        var output = (T*)Marshal.AllocHGlobal(sizeof(T) * array.Length);
        for (int i = 0; i < array.Length; ++i)
        {
            output[i] = array[i];
        }

        return new ManagedArrayWrapper((nint)output, array.Length);
    }

    public static unsafe ManagedArrayWrapper FromObjectArray<T>(T[] array) where T : Object
    {
        var output = (ManagedObjectReferenceWrapper*)Marshal.AllocHGlobal(sizeof(ManagedObjectReferenceWrapper) * array.Length);
        for (int i = 0; i < array.Length; ++i)
        {
            output[i] = array[i]?.AsManagedObjectReferenceWrapper() ?? default;
        }

        return new ManagedArrayWrapper((nint)output, array.Length);
    }

    public static unsafe ManagedArrayWrapper FromStringArray(string[] array)
    {
        int length = sizeof(int);
        for (int i = 0; i < array.Length; ++i)
        {
            length += sizeof(int);
            length += sizeof(char) * (array[i]?.Length ?? 0);
        }

        var output = (byte*)Marshal.AllocHGlobal(length);
        int seekpos = 0;

        BitConverter.TryWriteBytes(new Span<byte>(output, sizeof(int)), array.Length);
        seekpos += sizeof(int);

        for (int i = 0; i < array.Length; ++i)
        {
            int sl = array[i]?.Length ?? 0;
            BitConverter.TryWriteBytes(new Span<byte>(output + seekpos, sizeof(int)), sl);
            seekpos += sizeof(int);
            fixed (char* p = array[i])
            {
                int copySize = sizeof(char) * sl;
                Buffer.MemoryCopy(p, output + seekpos, copySize, copySize);
                seekpos += copySize;
            }
        }

        return new ManagedArrayWrapper((nint)output, length);
    }
}
