namespace AylaEngine.RHT;

internal class Vector3TypeName : TypeName
{
    public override string ToString() => CSharpName;

    public override string CppName => "::Ayla::Vector3D";

    public override string CppBindingName => CppName;

    public override string CSharpName => "global::Ayla.Vector3";

    public override string CSharpBindingName => CSharpName;

    public override string Id => "global::Ayla.Vector3";

    public override bool IsGenericTypeDefinition => false;
}
