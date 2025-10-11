namespace AylaEngine;

internal class QuaternionTypeName : TypeName
{
    public override string ToString() => CSharpName;

    public override string CppName => "::Ayla::QuaternionD";

    public override string CppBindingName => CppName;

    public override string CSharpName => "global::Ayla.Quaternion";

    public override string CSharpBindingName => CSharpName;

    public override string Id => "global::Ayla.Quaternion";

    public override bool IsGenericTypeDefinition => false;
}
