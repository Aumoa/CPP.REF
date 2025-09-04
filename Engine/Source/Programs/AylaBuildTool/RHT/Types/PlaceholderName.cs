namespace AylaEngine.RHT.Types;

internal class PlaceholderName : TypeName
{
    public override string CppName => string.Empty;

    public override string CppMemberName => string.Empty;

    public override string CppRootName => string.Empty;

    public override string BindingName => string.Empty;

    public override string CSharpName => string.Empty;

    public override string Id => string.Empty;

    public override bool IsGenericTypeDefinition => false;

    public static readonly PlaceholderName Value = new PlaceholderName();
}
