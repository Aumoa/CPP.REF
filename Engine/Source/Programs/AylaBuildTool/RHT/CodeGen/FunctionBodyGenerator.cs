namespace AylaEngine.RHT.CodeGen;

internal readonly struct FunctionBodyGenerator(IParameterCollection collection, string callable, TypeName returnType)
{
    public static string GeneratePassArguments<T>(T parameters) where T : IParameterCollection
    {
        return string.Join(", ", parameters.Parameters.Select(p => p.N));
    }

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
        GenerateCSharpBodyDefault(ref sourceCode, ref indent, formatLine, scoped, arguments, bodyStmt =>
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
        GenerateCSharpBodyDefault(ref sourceCode, ref indent, formatLine, scoped, arguments, bodyStmt =>
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

    public void GenerateCppNativeToCSharp(Action<string> formatLine)
    {
        List<string> scoped = [];
        List<string> arguments = [];

        foreach (var (typeName, name) in collection.Parameters)
        {
            if (typeName == TypeName.String)
            {
                scoped.Add($"auto {name}__wrapper = ::Ayla::ManagedStringWrapper::FromString({name});");
                arguments.Add($"{name}__wrapper");
            }
            else if (typeName is ArrayTypeName arrayType)
            {
                var elementType = arrayType.ElementType;
                if (elementType == TypeName.String)
                {
                    scoped.Add($"auto {name}__wrapper = ::Ayla::ManagedArrayWrapper::FromStringArray({name});");
                    arguments.Add($"{name}__wrapper");
                }
                else if (elementType is SharedPtrTypeName)
                {
                    scoped.Add($"auto {name}__wrapper = ::Ayla::ManagedArrayWrapper::FromObjectArray({name});");
                    arguments.Add($"{name}__wrapper");
                }
                else
                {
                    scoped.Add($"auto {name}__wrapper = ::Ayla::ManagedArrayWrapper::FromArray({name});");
                    arguments.Add($"{name}__wrapper");
                }
            }
            else if (typeName is SharedPtrTypeName)
            {
                scoped.Add($"auto {name}__wrapper = ::Ayla::ObjectReferenceWrapper::FromObject({name});");
                arguments.Add($"{name}__wrapper");
            }
            else
            {
                arguments.Add(name);
            }
        }

        var returnType_ = returnType;
        GenerateCppBodyDefault(formatLine, scoped, arguments, bodyStmt =>
        {
            if (returnType_ == TypeName.Void)
            {
                formatLine(bodyStmt + ";");
            }
            else if (returnType_ is SharedPtrTypeName ptype)
            {
                formatLine($"return {bodyStmt}.AsNative<{ptype.ElementType.CppName}>();");
            }
            else
            {
                formatLine($"return {bodyStmt};");
            }
        });
    }

    public void GenerateCppCSharpToNative(Action<string> formatLine)
    {
        List<string> scoped = [];
        List<string> arguments = [];

        foreach (var (typeName, name) in collection.Parameters)
        {
            if (typeName == TypeName.String)
            {
                arguments.Add($"{name}.AsString()");
            }
            else if (typeName is ArrayTypeName arrayType)
            {
                var elementType = arrayType.ElementType;
                if (elementType == TypeName.String)
                {
                    arguments.Add($"{name}.AsStringArray()");
                }
                else if (elementType is SharedPtrTypeName ptype)
                {
                    arguments.Add($"{name}.AsObjectArray<{ptype.ElementType.CppName}>()");
                }
                else
                {
                    arguments.Add($"{name}.AsArray<{elementType.CppName}>()");
                }
            }
            else if (typeName is SharedPtrTypeName ptype)
            {
                arguments.Add($"{name}.AsObject<{ptype.ElementType.CppName}>()");
            }
            else
            {
                arguments.Add(name);
            }
        }

        var returnType_ = returnType;
        GenerateCppBodyDefault(formatLine, scoped, arguments, bodyStmt =>
        {
            if (returnType_ == TypeName.Void)
            {
                formatLine(bodyStmt + ";");
            }
            else if (returnType_ is SharedPtrTypeName)
            {
                formatLine($"return {bodyStmt}.AsNative<{returnType_.CppName}>();");
            }
            else
            {
                formatLine($"return {bodyStmt};");
            }
        });
    }

    private void GenerateCppBodyDefault(Action<string> formatLine, List<string> scoped, List<string> arguments, Action<string> formatBodyStatement)
    {
        if (scoped.Count > 0)
        {
            foreach (var stmt in scoped)
            {
                formatLine(stmt);
            }
        }

        string bodyStmt = $"{callable}({string.Join(", ", arguments)})";
        formatBodyStatement(bodyStmt);
    }

    private void GenerateCSharpBodyDefault(ref string sourceCode, ref int indent, Func<string, string> formatLine, List<string> scoped, List<string> arguments, Func<string, string> formatBodyStatement)
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
