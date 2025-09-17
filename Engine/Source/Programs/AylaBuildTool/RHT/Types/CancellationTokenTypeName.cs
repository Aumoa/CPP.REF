namespace AylaEngine.RHT.Types;

internal class CancellationTokenTypeName : TypeName
{
    public override string ToString() => CSharpName;

    public override string CppName => "::Ayla::CancellationToken";

    public override string CppMemberName => CppName;

    public override string CppRootName => CppName;

    public override string CppBindingName => CppName;

    public override string CSharpName => "global::System.Threading.CancellationToken";

    public override string CSharpBindingName => CSharpName;

    public override string Id => "global::Ayla.CancellationToken";

    public override bool IsGenericTypeDefinition => false;
}
