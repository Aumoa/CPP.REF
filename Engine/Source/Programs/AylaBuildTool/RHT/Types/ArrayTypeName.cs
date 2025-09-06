namespace AylaEngine.RHT.Types;

internal class ArrayTypeName : TypeName
{
    public readonly TypeName ElementType;

    public ArrayTypeName(TypeName elementType)
    {
        ElementType = elementType;
    }

    public override string CppName => $"::std::vector<{ElementType.CppRootName}>";

    public override string CppMemberName => $"::std::vector<{ElementType.CppMemberName}>";

    public override string CppRootName => $"::std::vector<{ElementType.CppRootName}>";

    public override string CppBindingName => "void*";

    public override string CSharpName => $"{ElementType.CSharpName}[]";

    public override string CSharpBindingName => "nint";

    public override string Id => $"global::std.vector<{ElementType.Id}>";

    public override bool IsGenericTypeDefinition => ElementType is PlaceholderName;

    public static readonly ArrayTypeName GenericTypeDefinition = new ArrayTypeName(PlaceholderName.Value);
}
