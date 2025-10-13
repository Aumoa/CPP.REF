namespace AylaEngine;

internal class ManagedTypeName : TypeName
{
    public override string ToString() => CSharpName;

    public override string CppName => "::Ayla::ManagedTypeWrapper";

    public override string CppBindingName => CppName;

    public override string CSharpName => "global::Ayla.ManagedTypeWrapper";

    public override string CSharpBindingName => CSharpName;

    public override string Id => CSharpName;

    public override bool IsGenericTypeDefinition => false;
}
