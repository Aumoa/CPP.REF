using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public unsafe struct ManagedTypeWrapper
{
    public readonly nint NativeType;
    public readonly nint ScriptTypeGetter;

    public Type GetScriptType() => Marshal.GetDelegateForFunctionPointer<GetScriptTypeDelegate>(ScriptTypeGetter)();
}
