namespace AylaEngine;

internal class PlaceholderName : TypeName
{
    public override string CppName => string.Empty;

    public override string CppBindingName => string.Empty;

    public override string CSharpName => string.Empty;

    public override string CSharpBindingName => string.Empty;

    public override string Id => string.Empty;

    public override bool IsGenericTypeDefinition => false;

    public static readonly PlaceholderName Value = new PlaceholderName();
}
