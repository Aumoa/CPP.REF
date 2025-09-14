namespace AylaEngine;

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

    public override string CppMemberName => $"::Ayla::PPtr<{CppName}>";

    public override string CppRootName => $"::Ayla::RPtr<{CppName}>";

    public override string CppBindingName => "void*";

    public override string CSharpName => Namespace.CSharp(Name);

    public override string CSharpBindingName => "global::Ayla.ObjectReferenceWrapper";

    public override string Id => CSharpName;

    public override bool IsGenericTypeDefinition => false;
}
