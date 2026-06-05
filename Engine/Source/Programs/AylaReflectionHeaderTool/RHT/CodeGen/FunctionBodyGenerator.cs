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
                arguments.Add($"new global::Ayla.ManagedStringWrapper({name}_ptr, {name}?.Length ?? 0)");
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
                return formatLine($"global::Ayla.NativeException.ThrowIfFailed({bodyStmt});");
            }

            string source = string.Empty;
            source += formatLine($"{returnType_.CSharpBindingName} __return_value;");
            source += formatLine($"global::Ayla.NativeException.ThrowIfFailed({bodyStmt});");
            source += formatLine($"return {FormatCSharpReturnValue(returnType_, "__return_value")};");
            return source;
        }, true);
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

    public void GenerateCSharpNativeToCSharpStatus(ref string sourceCode, ref int indent, Func<string, string> formatLine)
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
            string source = string.Empty;
            if (returnType_ == TypeName.Void)
            {
                source += formatLine(bodyStmt + ";");
            }
            else if (returnType_ is SharedPtrTypeName)
            {
                source += formatLine($"__return_value = (global::Ayla.ObjectReferenceWrapper){bodyStmt};");
            }
            else
            {
                source += formatLine($"__return_value = {bodyStmt};");
            }

            source += formatLine("return global::Ayla.NativeCallStatus.Success;");
            return source;
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

    public void GenerateCppNativeToCSharpStatus(Action<string> formatLine, Action<Action> indented)
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
        if (returnType_ != TypeName.Void)
        {
            formatLine($"{returnType_.CppBindingName} __return_value{{}};");
            arguments.Add("&__return_value");
        }

        GenerateCppBodyDefault(formatLine, scoped, arguments, bodyStmt =>
        {
            formatLine($"auto __status = {bodyStmt};");
            formatLine("if (__status != ::Ayla::NativeCallStatus::Success)");
            formatLine("{");
            indented(() =>
            {
                formatLine("::Ayla::ManagedExceptionInterop::ThrowLastException();");
            });
            formatLine("}");

            if (returnType_ == TypeName.Void)
            {
                return;
            }
            else if (returnType_ == TypeName.String)
            {
                formatLine("return __return_value.AsString();");
            }
            else if (returnType_ is SharedPtrTypeName ptype)
            {
                formatLine($"return __return_value.AsNative<{ptype.ElementType.CppName}>();");
            }
            else
            {
                formatLine("return __return_value;");
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
                arguments.Add($"{name}.AsNative<{ptype.ElementType.CppName}>()");
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
            else if (returnType_ == TypeName.String)
            {
                formatLine($"return ::Ayla::ManagedStringWrapper::FromIntString({bodyStmt});");
            }
            else if (returnType_ is SharedPtrTypeName)
            {
                formatLine($"return ::Ayla::ObjectReferenceWrapper::FromObject({bodyStmt});");
            }
            else if (returnType_ is PlaceholderName)
            {
                formatLine($"return ({bodyStmt})->BindGCHandle__Unsafe(__gchandle_ptr);");
            }
            else
            {
                formatLine($"return {bodyStmt};");
            }
        });
    }

    public void GenerateCppCSharpToNativeStatus(Action<string> formatLine)
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
                arguments.Add($"{name}.AsNative<{ptype.ElementType.CppName}>()");
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
            else if (returnType_ == TypeName.String)
            {
                formatLine($"*__return_value = ::Ayla::ManagedStringWrapper::FromIntString({bodyStmt});");
            }
            else if (returnType_ is SharedPtrTypeName)
            {
                formatLine($"*__return_value = ::Ayla::ObjectReferenceWrapper::FromObject({bodyStmt});");
            }
            else if (returnType_ is PlaceholderName)
            {
                formatLine($"*__return_value = ({bodyStmt})->BindGCHandle__Unsafe(__gchandle_ptr);");
            }
            else
            {
                formatLine($"*__return_value = {bodyStmt};");
            }

            formatLine("return ::Ayla::NativeCallStatus::Success;");
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

    private static string FormatCSharpReturnValue(TypeName typeName, string value)
    {
        if (typeName == TypeName.String)
        {
            return $"{value}.AsManaged()";
        }

        if (typeName is SharedPtrTypeName)
        {
            return $"{value}.AsManaged<{typeName.CSharpName}>()";
        }

        return value;
    }

    private void GenerateCSharpBodyDefault(ref string sourceCode, ref int indent, Func<string, string> formatLine, List<string> scoped, List<string> arguments, Func<string, string> formatBodyStatement, bool appendOutReturn = false)
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

            string bodyStmt = appendOutReturn && returnType != TypeName.Void
                ? $"{callable}({string.Join(", ", arguments.Append("out __return_value"))})"
                : $"{callable}({string.Join(", ", arguments)})";
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
