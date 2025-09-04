namespace AylaEngine.RHT.Types;

internal class RPtrTypeName : TypeName
{
    public readonly TypeName ElementType;

    public RPtrTypeName(TypeName elementType)
    {
        ElementType = elementType;
    }

    public override string CppName => $"::Ayla::RPtr<{ElementType.CppRootName}>";

    public override string CppMemberName => $"::Ayla::PPtr<{ElementType.CppMemberName}>";

    public override string CppRootName => $"::Ayla::RPtr<{ElementType.CppRootName}>";

    public override string BindingName => "void*";

    public override string CSharpName => ElementType.CSharpName;

    public override string Id => $"global::Ayla.RPtr<{ElementType.Id}>";

    public override bool IsGenericTypeDefinition => ElementType is PlaceholderName;

    public static readonly RPtrTypeName GenericTypeDefinition = new RPtrTypeName(PlaceholderName.Value);
}
