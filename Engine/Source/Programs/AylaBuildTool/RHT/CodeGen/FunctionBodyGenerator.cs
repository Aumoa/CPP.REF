namespace AylaEngine.RHT.CodeGen;

internal readonly struct FunctionBodyGenerator(IParameterCollection collection, string callable, TypeName returnType)
{
    public void GenerateCSharpCSharpToNative(ref string sourceCode, ref int indent, Func<string, string> formatLine)
    {
        List<string> scoped = [];
        List<string> arguments = [];

        foreach (var (typeName, name) in collection.Parameters)
        {
            if (typeName == TypeName.String)
            {
                scoped.Add($"fixed (char* {name}_ptr = {name})");
                arguments.Add($"new global::Ayla.ManagedStringWrapper({name}_ptr, {name}.Length)");
            }
            else if (typeName is ArrayTypeName arrayType)
            {
                var elementType = arrayType.ElementType;
                if (elementType == TypeName.String)
                {
                    scoped.Add($"using (var {name}_wrapper = global::Ayla.ManagedArrayWrapper.FromStringArray({name}))");
                    arguments.Add($"{name}_wrapper");
                }
                else if (elementType is SharedPtrTypeName)
                {
                    scoped.Add($"using (var {name}_wrapper = global::Ayla.ManagedArrayWrapper.FromObjectArray({name}))");
                    arguments.Add($"{name}_wrapper");
                }
                else
                {
                    scoped.Add($"fixed ({elementType.CSharpBindingName}* {name}_ptr = {name})");
                    arguments.Add($"new global::Ayla.ManagedArrayWrapper((nint){name}_ptr, {name}.Length)");
                }
            }
            else
            {
                arguments.Add(name);
            }
        }

        var returnType_ = returnType;
        GenerateDefaultBody(ref sourceCode, ref indent, formatLine, scoped, arguments, bodyStmt =>
        {
            if (returnType_ == TypeName.Void)
            {
                return formatLine(bodyStmt + ";");
            }
            else if (returnType_ is SharedPtrTypeName)
            {
                return formatLine($"return {bodyStmt}.AsManaged<{returnType_.CSharpName}>();");
            }
            else
            {
                return formatLine($"return {bodyStmt};");
            }
        });
    }

    public void GenerateCSharpNativeToCSharp(ref string sourceCode, ref int indent, Func<string, string> formatLine)
    {
        List<string> scoped = [];
        List<string> arguments = [];

        foreach (var (typeName, name) in collection.Parameters)
        {
            if (typeName == TypeName.String)
            {
                arguments.Add($"{name}.AsManaged()");
            }
            else if (typeName is ArrayTypeName arrayType)
            {
                var elementType = arrayType.ElementType;
                if (elementType == TypeName.String)
                {
                    arguments.Add($"{name}.AsStringArray()");
                }
                else if (elementType is SharedPtrTypeName)
                {
                    arguments.Add($"{name}.AsObjectArray<{elementType.CSharpName}>()");
                }
                else
                {
                    arguments.Add($"{name}.AsArray<{elementType.CSharpName}>()");
                }
            }
            else if (typeName is SharedPtrTypeName)
            {
                arguments.Add($"{name}.AsManaged<{typeName.CSharpName}>()");
            }
            else
            {
                arguments.Add(name);
            }
        }

        var returnType_ = returnType;
        GenerateDefaultBody(ref sourceCode, ref indent, formatLine, scoped, arguments, bodyStmt =>
        {
            if (returnType_ == TypeName.Void)
            {
                return formatLine(bodyStmt + ";");
            }
            else if (returnType_ is SharedPtrTypeName)
            {
                return formatLine($"return (global::Ayla.ObjectReferenceWrapper){bodyStmt};");
            }
            else
            {
                return formatLine($"return {bodyStmt};");
            }
        });
    }

    public static string GeneratePassArguments<T>(T parameters) where T : IParameterCollection
    {
        return string.Join(", ", parameters.Parameters.Select(p => p.N));
    }

    private void GenerateDefaultBody(ref string sourceCode, ref int indent, Func<string, string> formatLine, List<string> scoped, List<string> arguments, Func<string, string> formatBodyStatement)
    {
        int localIndent = 0;

        try
        {
            if (scoped.Count > 0)
            {
                foreach (var stmt in scoped)
                {
                    sourceCode += formatLine(stmt);
                }

                sourceCode += formatLine("{");
                Indent(ref indent);
            }

            string bodyStmt = $"{callable}({string.Join(", ", arguments)})";
            sourceCode += formatBodyStatement(bodyStmt);

            if (scoped.Count > 0)
            {
                Dedent(ref indent);
                sourceCode += formatLine("}");
            }
        }
        finally
        {
            while (localIndent > 0)
            {
                Dedent(ref indent);
            }
        }

        void Indent(ref int indent)
        {
            ++localIndent;
            ++indent;
        }

        void Dedent(ref int indent)
        {
            --localIndent;
            --indent;
        }
    }
}
