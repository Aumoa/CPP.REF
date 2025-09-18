namespace AylaEngine;

internal class SharedPtrTypeName : TypeName
{
    public readonly TypeName ElementType;

    public SharedPtrTypeName(TypeName elementType)
    {
        ElementType = elementType;
    }

    public override string CppName => $"::std::shared_ptr<{ElementType.CppName}>";

    public override string CppBindingName => "::Ayla::ObjectReferenceWrapper";

    public override string CSharpName => ElementType.CSharpName;

    public override string CSharpBindingName => "global::Ayla.ObjectReferenceWrapper";

    public override string Id => $"global::std.shared_ptr<{ElementType.Id}>";

    public override bool IsGenericTypeDefinition => ElementType is PlaceholderName;

    public static readonly SharedPtrTypeName GenericTypeDefinition = new(PlaceholderName.Value);

    public static readonly SharedPtrTypeName SharedObject = new(TypeName.Object);
}
