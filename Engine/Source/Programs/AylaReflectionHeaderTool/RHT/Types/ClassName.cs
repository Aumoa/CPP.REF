namespace AylaEngine.RHT;

internal class ClassName : TypeName
{
    public readonly NamespaceName Namespace;
    public readonly string Name;
    public readonly RHTGenerator Source;

    public ClassName(NamespaceName @namespace, string name, RHTGenerator source)
    {
        Namespace = @namespace;
        Name = name;
        Source = source;
    }

    public override string ToString() => CSharpName;

    public override string CppName => Namespace.Cpp(Name);

    public override string CppBindingName => throw AmbiguousObjectBindingDirection();

    public override string CppNativeToManagedBindingName => "::Ayla::NativeObjectReferenceWrapper";

    public override string CppManagedToNativeBindingName => "::Ayla::ManagedObjectReferenceWrapper";

    public override string CSharpName => Namespace.CSharp(Name);

    public override string CSharpBindingName => throw AmbiguousObjectBindingDirection();

    public override string CSharpNativeToManagedBindingName => "global::Ayla.NativeObjectReferenceWrapper";

    public override string CSharpManagedToNativeBindingName => "global::Ayla.ManagedObjectReferenceWrapper";

    public override string Id => CSharpName;

    public override bool IsGenericTypeDefinition => false;

    private static InvalidOperationException AmbiguousObjectBindingDirection()
        => new InvalidOperationException("Object binding requires an explicit interop direction.");
}
