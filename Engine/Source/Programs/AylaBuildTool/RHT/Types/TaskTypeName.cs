namespace AylaEngine;

internal class TaskTypeName : TypeName
{
    public readonly TypeName ElementType;

    public TaskTypeName(TypeName elementType)
    {
        ElementType = elementType;
    }

    public override string CppName => $"::Ayla::Task<{ElementType.CppName}>";

    public override string CppBindingName => "::Ayla::ManagedTaskWrapper";

    public override string CSharpName => ElementType == TypeName.Void ? "global::System.Threading.Tasks.Task" : $"global::System.Threading.Tasks.Task<{ElementType.CSharpName}>";

    public override string CSharpBindingName => "global::Ayla.ManagedTaskWrapper";

    public override string Id => $"global::Ayla.Task<{ElementType.Id}>";

    public override bool IsGenericTypeDefinition => ElementType is PlaceholderName;

    public static readonly TaskTypeName GenericTypeDefinition = new TaskTypeName(PlaceholderName.Value);
}
