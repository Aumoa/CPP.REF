namespace AylaEngine;

internal abstract class TypeName
{
    public abstract string CppName { get; }

    public abstract string CppBindingName { get; }

    public abstract string CSharpName { get; }

    public abstract string CSharpBindingName { get; }

    public abstract string Id { get; }

    public abstract bool IsGenericTypeDefinition { get; }

    public override string ToString() => Id;

    public static readonly TypeName Void = new BuiltinTypeName(BuiltinTypeName.Kinds.Void);
    public static readonly TypeName Boolean = new BuiltinTypeName(BuiltinTypeName.Kinds.Boolean);
    public static readonly TypeName Int32 = new BuiltinTypeName(BuiltinTypeName.Kinds.Int32);
    public static readonly TypeName Int64 = new BuiltinTypeName(BuiltinTypeName.Kinds.Int64);
    public static readonly TypeName UInt32 = new BuiltinTypeName(BuiltinTypeName.Kinds.UInt32);
    public static readonly TypeName UInt64 = new BuiltinTypeName(BuiltinTypeName.Kinds.UInt64);
    public static readonly TypeName IntPtr = new BuiltinTypeName(BuiltinTypeName.Kinds.IntPtr);
    public static readonly TypeName UIntPtr = new BuiltinTypeName(BuiltinTypeName.Kinds.UIntPtr);
    public static readonly TypeName String = new BuiltinTypeName(BuiltinTypeName.Kinds.String);
    public static readonly TypeName Single = new BuiltinTypeName(BuiltinTypeName.Kinds.Single);
    public static readonly TypeName Double = new BuiltinTypeName(BuiltinTypeName.Kinds.Double);
    public static readonly TypeName Object = new BuiltinTypeName(BuiltinTypeName.Kinds.Object);
    public static readonly CancellationTokenTypeName CancellationToken = new CancellationTokenTypeName();
    public static readonly Vector2TypeName Vector2 = new Vector2TypeName();
    public static readonly Vector3TypeName Vector3 = new Vector3TypeName();
    public static readonly Vector4TypeName Vector4 = new Vector4TypeName();
    public static readonly QuaternionTypeName Quaternion = new QuaternionTypeName();
}
