namespace AylaEngine;

internal class TypeNames(TypeName[] customTypeNames)
{
    public readonly TypeName[] All =
    [
        TypeName.Void,
        TypeName.Boolean,
        TypeName.Int32,
        TypeName.Int64,
        TypeName.UInt32,
        TypeName.UInt64,
        TypeName.IntPtr,
        TypeName.UIntPtr,
        TypeName.String,
        TypeName.Single,
        TypeName.Double,
        TypeName.Object,
        ArrayTypeName.GenericTypeDefinition,
        SharedPtrTypeName.GenericTypeDefinition,
        TypeName.CancellationToken,
        TypeName.ManagedType,
        TypeName.Vector2,
        TypeName.Vector3,
        TypeName.Vector4,
        TypeName.Quaternion,
        .. customTypeNames
    ];

    public ClassName FindClass(SClass @class)
    {
        string tid = @class.Namespaces.Length == 0
            ? "global::" + @class.Name
            : "global::" + string.Join(".", @class.Namespaces.Select(p => p.Name)) + "." + @class.Name;

        var found = All.OfType<ClassName>().FirstOrDefault(t => t.Id == tid);
        if (found == null)
        {
            throw @class.Context.ParsingError($"Type Error: The type '{tid}' is not registered as a known type.");
        }

        return found;
    }

    public EnumName FindEnum(SAEnum @enum)
    {
        string tid = @enum.Namespaces.Length == 0
            ? "global::" + @enum.Name
            : "global::" + string.Join(".", @enum.Namespaces.Select(p => p.Name)) + "." + @enum.Name;

        var found = All.OfType<EnumName>().FirstOrDefault(t => t.Id == tid);
        if (found == null)
        {
            throw @enum.Context.ParsingError($"Type Error: The type '{tid}' is not registered as a known type.");
        }

        return found;
    }

    public TypeName FindType(STypeName typeName, SClass declaringType)
    {
        using var iterator = ((IList<STypeName.Part>)typeName.NameParts).GetEnumerator();
        return FindType(typeName, iterator, declaringType);
    }

    private TypeName FindType(STypeName typeName, IEnumerator<STypeName.Part> iterator, SClass declaringType)
    {
        List<STypeName.Part> parts = [];
        List<TypeName> genericArguments = [];
        while (iterator.MoveNext())
        {
            var current = iterator.Current;
            if (current.Type == STypeName.PartType.TemplateEnd || current.Type == STypeName.PartType.TemplateSeparator)
            {
                break;
            }
            else if (current.Type == STypeName.PartType.TemplateBegin)
            {
                parts.Add(current);
                genericArguments.Add(FindType(typeName, iterator, declaringType));
                parts.Add(iterator.Current);
            }
            else
            {
                parts.Add(current);
            }
        }

        TypeName? found = null;
        var csName = string.Concat(parts.Select(PartAsName));
        if (csName[0] == '.')
        {
            csName = "global::" + csName[1..];
            var global = "global::" + csName;
            found = All.FirstOrDefault(t => t.Id == global);
        }
        else
        {
            for (int i = 0; i < declaringType.Namespaces.Length + 1; ++i)
            {
                var nestedNamespaces = declaringType.Namespaces.Take(declaringType.Namespaces.Length - i).Select(n => n.Name);
                var nestedName = new NamespaceName(nestedNamespaces.ToArray()).CSharp(csName);
                found = All.FirstOrDefault(t => t.Id == nestedName);
                if (found != null)
                {
                    break;
                }
            }

            if (found == null)
            {
                // Find in the default namespace.
                var nestedName = new NamespaceName("Ayla").CSharp(csName);
                found = All.FirstOrDefault(t => t.Id == nestedName);
            }
        }

        if (found == null)
        {
            throw typeName.Context.ParsingError($"Type Error: The type '{typeName.FullName}' is not registered as a known type.");
        }

        if (found.IsGenericTypeDefinition)
        {
            return (TypeName)Activator.CreateInstance(found.GetType(), genericArguments.ToArray())!;
        }
        else
        {
            return found;
        }

        static string PartAsName(STypeName.Part part)
        {
            return part.Type switch
            {
                STypeName.PartType.NamespaceSeparator => ".",
                STypeName.PartType.TemplateSeparator => ",",
                STypeName.PartType.TemplateBegin => "<",
                STypeName.PartType.TemplateEnd => ">",
                _ => part.Name!
            };
        }
    }
}
