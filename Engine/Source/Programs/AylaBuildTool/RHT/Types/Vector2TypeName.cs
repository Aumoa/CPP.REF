namespace AylaEngine.RHT;

internal class Vector2TypeName : TypeName
{
    public override string ToString() => CSharpName;

    public override string CppName => "::Ayla::Vector2D";

    public override string CppBindingName => CppName;

    public override string CSharpName => "global::Ayla.Vector2";

    public override string CSharpBindingName => CSharpName;

    public override string Id => "global::Ayla.Vector2";

    public override bool IsGenericTypeDefinition => false;
}
