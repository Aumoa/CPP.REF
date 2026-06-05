namespace AylaEngine.RHT;

internal class Vector4TypeName : TypeName
{
    public override string ToString() => CSharpName;

    public override string CppName => "::Ayla::Vector4D";

    public override string CppBindingName => CppName;

    public override string CSharpName => "global::Ayla.Vector4";

    public override string CSharpBindingName => CSharpName;

    public override string Id => "global::Ayla.Vector4";

    public override bool IsGenericTypeDefinition => false;
}
