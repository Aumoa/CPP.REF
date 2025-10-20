using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

[StructLayout(LayoutKind.Sequential, Pack = 8)]
public unsafe struct ManagedTypeWrapper
{
    public nint NativeType;
    public nint ScriptTypeGetter;

    public Type GetScriptType() => Marshal.GetDelegateForFunctionPointer<GetScriptTypeDelegate>(ScriptTypeGetter)();

    private static Dictionary<Type, (nint, GetScriptTypeDelegate)> s_NativeTypeMapper = [];

    public static implicit operator ManagedTypeWrapper(Type type)
    {
        (nint, GetScriptTypeDelegate) value;
        lock (s_NativeTypeMapper)
        {
            if (s_NativeTypeMapper.TryGetValue(type, out value) == false)
            {
                var staticClassMethod = GetStaticClassMethod(type);
                var managedTypeWrapper = (ManagedTypeWrapper)staticClassMethod.Invoke(null, [])!;
                value = (managedTypeWrapper.NativeType, () => type);
                s_NativeTypeMapper.Add(type, value);
            }
        }

        return new ManagedTypeWrapper
        {
            NativeType = value.Item1,
            ScriptTypeGetter = Marshal.GetFunctionPointerForDelegate(value.Item2)
        };
    }

    private static MethodInfo GetStaticClassMethod(Type? type)
    {
        while (type != null)
        {
            var methodInfo = type.GetMethod("StaticClass", BindingFlags.Public | BindingFlags.Static);
            if (methodInfo != null)
            {
                return methodInfo;
            }

            type = type.BaseType;
        }

        throw new InvalidOperationException("No StaticClass method found in type hierarchy.");
    }
}
