namespace AylaEngine.RHT.Types;

internal class PPtrTypeName : TypeName
{
    public readonly TypeName ElementType;

    public PPtrTypeName(TypeName elementType)
    {
        ElementType = elementType;
    }

    public override string CppName => $"::Ayla::PPtr<{ElementType.CppRootName}>";

    public override string CppMemberName => $"::Ayla::PPtr<{ElementType.CppMemberName}>";

    public override string CppRootName => $"::Ayla::RPtr<{ElementType.CppRootName}>";

    public override string BindingName => "void*";

    public override string CSharpName => ElementType.CSharpName;

    public override string Id => $"global::Ayla.PPtr<{ElementType.Id}>";

    public override bool IsGenericTypeDefinition => ElementType is PlaceholderName;

    public static readonly PPtrTypeName GenericTypeDefinition = new PPtrTypeName(PlaceholderName.Value);
}
