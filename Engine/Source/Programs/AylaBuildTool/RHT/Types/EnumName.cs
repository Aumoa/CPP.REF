namespace AylaEngine;

internal class EnumName : TypeName
{
    public readonly NamespaceName Namespace;
    public readonly string Name;
    public readonly RHTGenerator Source;

    public EnumName(NamespaceName @namespace, string name, RHTGenerator source)
    {
        Namespace = @namespace;
        Name = name;
        Source = source;
    }

    public override string ToString() => CSharpName;

    public override string CppName => Namespace.Cpp(Name);

    public override string CppMemberName => CppName;

    public override string CppRootName => CppName;

    public override string CppBindingName => CppName;

    public override string CSharpName => Namespace.CSharp(Name);

    public override string CSharpBindingName => CSharpName;

    public override string Id => CSharpName;

    public override bool IsGenericTypeDefinition => false;
}
