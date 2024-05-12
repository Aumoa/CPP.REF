namespace AylaEngine;

internal static class Internals
{
    private struct InstancePtrScope : IDisposable
    {
        public InstancePtrScope(ulong instancePtr)
        {
            s_InstancePtr = instancePtr;
        }

        public void Dispose()
        {
            s_InstancePtr = 0;
        }
    }

    internal static ulong s_InstancePtr;

    public static T New<T>(ulong instancePtr) where T : Object, new()
    {
        using (new InstancePtrScope(instancePtr))
        {
            return new T();
        }
    }
}
