namespace AylaEngine.RHT;

internal class BuiltinTypeName : TypeName
{
    public enum Kinds
    {
        Void,
        Boolean,
        Int32,
        Int64,
        UInt32,
        UInt64,
        IntPtr,
        UIntPtr,
        String,
        Single,
        Double,
        Object
    }

    public readonly Kinds Kind;

    public BuiltinTypeName(Kinds kind)
    {
        Kind = kind;
    }

    public override string ToString() => CSharpName;

    public override string CppName => Kind switch
    {
        Kinds.Void => "void",
        Kinds.Boolean => "bool",
        Kinds.Int32 => "::Ayla::int32",
        Kinds.Int64 => "::Ayla::int64",
        Kinds.UInt32 => "::Ayla::uint32",
        Kinds.UInt64 => "::Ayla::uint64",
        Kinds.IntPtr => "::Ayla::ssize_t",
        Kinds.UIntPtr => "::Ayla::size_t",
        Kinds.String => "::Ayla::String",
        Kinds.Single => "float",
        Kinds.Double => "double",
        Kinds.Object => "::Ayla::Object",
        _ => throw UnsupportedKind()
    };

    public override string CppBindingName => Kind switch
    {
        Kinds.Void => "void",
        Kinds.String => "::Ayla::ManagedStringWrapper",
        Kinds.Object => "::Ayla::ObjectReferenceWrapper",
        _ => CppName
    };

    public override string CppNativeToManagedBindingName => Kind switch
    {
        Kinds.Object => "::Ayla::NativeObjectReferenceWrapper",
        _ => CppBindingName
    };

    public override string CppManagedToNativeBindingName => Kind switch
    {
        Kinds.Object => "::Ayla::ManagedObjectReferenceWrapper",
        _ => CppBindingName
    };

    public override string CSharpName => Kind switch
    {
        Kinds.Void => "void",
        Kinds.Boolean => "bool",
        Kinds.Int32 => "int",
        Kinds.Int64 => "long",
        Kinds.UInt32 => "uint",
        Kinds.UInt64 => "ulong",
        Kinds.IntPtr => "nint",
        Kinds.UIntPtr => "nuint",
        Kinds.String => "string",
        Kinds.Single => "float",
        Kinds.Double => "double",
        Kinds.Object => "global::Ayla.Object",
        _ => throw UnsupportedKind()
    };

    public override string CSharpBindingName => Kind switch
    {
        Kinds.Void => "void",
        Kinds.Boolean => "bool",
        Kinds.Int32 => "int",
        Kinds.Int64 => "long",
        Kinds.UInt32 => "uint",
        Kinds.UInt64 => "ulong",
        Kinds.IntPtr => "nint",
        Kinds.UIntPtr => "nuint",
        Kinds.String => "global::Ayla.ManagedStringWrapper",
        Kinds.Single => "float",
        Kinds.Double => "double",
        Kinds.Object => "global::Ayla.ObjectReferenceWrapper",
        _ => throw UnsupportedKind()
    };

    public override string CSharpNativeToManagedBindingName => Kind switch
    {
        Kinds.Object => "global::Ayla.NativeObjectReferenceWrapper",
        _ => CSharpBindingName
    };

    public override string CSharpManagedToNativeBindingName => Kind switch
    {
        Kinds.Object => "global::Ayla.ManagedObjectReferenceWrapper",
        _ => CSharpBindingName
    };

    public override string Id => Kind switch
    {
        Kinds.Void => "global::void",
        Kinds.Boolean => "global::bool",
        Kinds.Int32 => "global::Ayla.int32",
        Kinds.Int64 => "global::Ayla.int64",
        Kinds.UInt32 => "global::Ayla.uint32",
        Kinds.UInt64 => "global::Ayla.uint64",
        Kinds.IntPtr => "global::Ayla.ssize_t",
        Kinds.UIntPtr => "global::Ayla.size_t",
        Kinds.String => "global::Ayla.String",
        Kinds.Single => "global::float",
        Kinds.Double => "global::double",
        Kinds.Object => "global::Ayla.Object",
        _ => throw UnsupportedKind()
    };

    public override bool IsGenericTypeDefinition => false;

    private InvalidOperationException UnsupportedKind() => new InvalidOperationException($"Unsupported builtin type kind '{Kind}'.");
}
