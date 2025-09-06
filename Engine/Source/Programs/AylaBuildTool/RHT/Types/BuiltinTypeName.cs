namespace AylaEngine.RHT.Types;

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
        _ => throw TerminateException.Arguments()
    };

    public override string CppMemberName => Kind switch
    {
        Kinds.Void => throw User("A type cannot be void type."),
        Kinds.Object => "::Ayla::PPtr<::Ayla::Object>",
        _ => CppName
    };

    public override string CppRootName => Kind switch
    {
        Kinds.Object => "::Ayla::RPtr<::Ayla::Object>",
        _ => CppName
    };

    public override string BindingName => Kind switch
    {
        Kinds.Void => "void",
        Kinds.String => "const wchar_t*",
        Kinds.Object => "void*",
        _ => CppName
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
        _ => throw TerminateException.Arguments()
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
        _ => throw TerminateException.Arguments()
    };

    public override bool IsGenericTypeDefinition => false;

    private static TerminateException User(string message)
    {
        Console.Error.WriteLine(message);
        throw TerminateException.User();
    }
}
