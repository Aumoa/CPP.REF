namespace AylaEngine.RHT;

internal class SharedPtrTypeName : TypeName
{
    public readonly TypeName ElementType;

    public SharedPtrTypeName(TypeName elementType)
    {
        ElementType = elementType;
    }

    public override string CppName => $"::Ayla::SharedPtr<{ElementType.CppName}>";

    public override string CppBindingName => throw AmbiguousObjectBindingDirection();

    public override string CppNativeToManagedBindingName => "::Ayla::NativeObjectReferenceWrapper";

    public override string CppManagedToNativeBindingName => "::Ayla::ManagedObjectReferenceWrapper";

    public override string CSharpName => ElementType.CSharpName;

    public override string CSharpBindingName => throw AmbiguousObjectBindingDirection();

    public override string CSharpNativeToManagedBindingName => "global::Ayla.NativeObjectReferenceWrapper";

    public override string CSharpManagedToNativeBindingName => "global::Ayla.ManagedObjectReferenceWrapper";

    public override string Id => $"global::Ayla.SharedPtr<{ElementType.Id}>";

    public override bool IsGenericTypeDefinition => ElementType is PlaceholderName;

    public static readonly SharedPtrTypeName GenericTypeDefinition = new(PlaceholderName.Value);

    public static readonly SharedPtrTypeName SharedObject = new(TypeName.Object);

    private static InvalidOperationException AmbiguousObjectBindingDirection()
        => new InvalidOperationException("Object binding requires an explicit interop direction.");
}
