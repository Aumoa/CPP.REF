// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C++ class source code.
/// </summary>
internal class CppClassGenerator
{
    private readonly CppSourceGenerator m_Parent;
    private readonly SAClass m_Class;

    public CppClassGenerator(CppSourceGenerator parent, SAClass aclass)
    {
        m_Parent = parent;
        m_Class = aclass;
    }

    public void Generate()
    {
        string @namespace = string.Join("::", m_Class.Class.Namespaces.Select(p => p.Name));
        string className = m_Class.Class.Name;
        var classType = m_Parent.TypeNames.FindClass(m_Class.Class);

        m_Parent.WriteIndentedLine($"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {className});");
        m_Parent.WriteIndentedLine("");

        if (className == "Object")
        {
            return;
        }

        m_Parent.WriteIndentedLine($"namespace {@namespace}");
        m_Parent.WriteIndentedLine("{");
        m_Parent.Indented(() =>
        {
            GenerateConstructors(@namespace, className);
            GenerateGatherProperties(className);
            GenerateGetManagedType(className, classType);
            GenerateVirtualFunctions(@namespace, className, classType);
        });
        m_Parent.WriteIndentedLine("}");
        m_Parent.WriteIndentedLine("");

        GenerateExternCBlock(@namespace, className, classType);
    }

    private void GenerateConstructors(string @namespace, string className)
    {
        for (int i = 0; i < m_Class.Constructors.Count; ++i)
        {
            var constructor = m_Class.Constructors[i];
            var parameterDeclare = string.Join(", ", constructor.Parameters.Select(p => p.Variable.TypeName.FullName + " " + p.Variable.Name));

            m_Parent.WriteIndentedLine($"::Ayla::SharedPtr<::Ayla::Object> {className}::reflexpr_class::members::constructor__{i}__{constructor.Name}({parameterDeclare})");
            m_Parent.WriteIndentedLine("{");
            m_Parent.Indented(() =>
            {
                var arguments = string.Join(", ", constructor.Parameters.Select(p => p.Variable.Name));
                m_Parent.WriteIndentedLine($"return ::Ayla::Object::UnsafeNew<{m_Class.Class.Name}>({arguments});");
            });
            m_Parent.WriteIndentedLine("}");
        }
    }

    private void GenerateGatherProperties(string className)
    {
        m_Parent.WriteIndentedLine("");
        m_Parent.WriteIndentedLine($"void {className}::GatherProperties(::Ayla::PropertyCollector& collector)");
        m_Parent.WriteIndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_Parent.WriteIndentedLine($"Super::GatherProperties(collector);");
            m_Parent.WriteIndentedLine($"Transfer(collector);");
        });
        m_Parent.WriteIndentedLine("}");
    }

    private void GenerateGetManagedType(string className, ClassName classType)
    {
        m_Parent.WriteIndentedLine("");
        m_Parent.WriteIndentedLine($"::Ayla::ManagedTypeWrapper {className}::GetManagedType()");
        m_Parent.WriteIndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_Parent.WriteIndentedLine($"using signature_t = void*(*)();");
            m_Parent.WriteIndentedLine($"static ::Ayla::ManagedTypeWrapper s_Type =");
            m_Parent.WriteIndentedLine("{");
            m_Parent.Indented(() =>
            {
                m_Parent.WriteIndentedLine($".NativeType = ::Ayla::TypeCollector::FindType(typeid({className})),");
                m_Parent.WriteIndentedLine($".ScriptTypeGetter = reinterpret_cast<signature_t>(::Ayla::ScriptingBackend::Get().GetFunctionPointer(\"{m_Parent.Context.ScriptAssemblyName}\", \"{classType.CSharpName["global::".Length..]}__Invocable\", \"GetScriptType__Invoke\"))()");
            });
            m_Parent.WriteIndentedLine("};");
            m_Parent.WriteIndentedLine("");
            m_Parent.WriteIndentedLine($"return s_Type;");
        });
        m_Parent.WriteIndentedLine("}");
    }

    private void GenerateVirtualFunctions(string @namespace, string className, ClassName classType)
    {
        m_Parent.WriteIndentedLine("");

        for (int i = 0; i < m_Class.Functions.Count; ++i)
        {
            var function = m_Class.Functions[i];
            bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);
            if (!isVirtual)
            {
                continue;
            }

            var parameters = CollectParameters(function.Parameters);
            var returnType = m_Parent.TypeNames.FindType(function.ReturnType, m_Class.Class);
            var parametersDeclare = ParametersGenerator.GenerateCpp(parameters);
            var suffix = function.Flags.HasFlag(SFunction.FFlags.Const) ? " const" : string.Empty;

            m_Parent.WriteIndentedLine($"{function.ReturnType.FullName} {className}::{function.Name}({parametersDeclare}){suffix}");
            m_Parent.WriteIndentedLine("{");
            m_Parent.Indented(() =>
            {
                var invokeParametersDeclare = ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.Object, "self_"));
                m_Parent.WriteIndentedLine($"using signature_t = {returnType.CppBindingName}(*)({invokeParametersDeclare});");
                m_Parent.WriteIndentedLine($"static auto callable = reinterpret_cast<signature_t>(::Ayla::ScriptingBackend::Get().GetFunctionPointer(\"{m_Parent.Context.ScriptAssemblyName}\", \"{classType.CSharpName["global::".Length..]}__Invocable\", \"{function.Name}__Invoke\"));");
                m_Parent.WriteIndentedLine($"auto self = AsShared();");

                string callable = "callable";
                var codeGen = new FunctionBodyGenerator(parameters.AddFirstTemp(SharedPtrTypeName.SharedObject, "self"), callable, returnType);
                codeGen.GenerateCppNativeToCSharp(m_Parent.WriteIndentedLine);
            });
            m_Parent.WriteIndentedLine("}");

            if (m_Parent.Context.AllowsNativeFunctionInvocation == false)
            {
                m_Parent.WriteIndentedLine($"{function.ReturnType.FullName} {className}::{function.Name}_Implementation({parametersDeclare})");
                m_Parent.WriteIndentedLine("{");
                m_Parent.Indented(() =>
                {
                    m_Parent.WriteIndentedLine($"throw ::Ayla::AccessViolationException(TEXT(\"Assemblies of the Application or Console type cannot directly invoke native functions.\"));");
                });
                m_Parent.WriteIndentedLine("}");
            }
        }
    }

    private void GenerateExternCBlock(string @namespace, string className, ClassName classType)
    {
        m_Parent.WriteIndentedLine($"extern \"C\"");
        m_Parent.WriteIndentedLine("{");
        m_Parent.Indented(() =>
        {
            GenerateGetManagedTypeExport(@namespace, className, classType);
            GenerateConstructorExports(@namespace, className, classType);
            GenerateFunctionExports(@namespace, className, classType);
        });
        m_Parent.WriteIndentedLine("}");
        m_Parent.WriteIndentedLine("");
    }

    private void GenerateGetManagedTypeExport(string @namespace, string className, ClassName classType)
    {
        m_Parent.WriteIndentedLine($"PLATFORM_SHARED_EXPORT ::Ayla::ManagedTypeWrapper {classType.CppName[2..].Replace("::", "__")}__GetManagedType()");
        m_Parent.WriteIndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_Parent.WriteIndentedLine($"return {@namespace}::{className}::GetManagedType();");
        });
        m_Parent.WriteIndentedLine("}");
        m_Parent.WriteIndentedLine("");
    }

    private void GenerateConstructorExports(string @namespace, string className, ClassName classType)
    {
        for (int i = 0; i < m_Class.Constructors.Count; ++i)
        {
            var constructor = m_Class.Constructors[i];
            var parameters = CollectParameters(constructor.Parameters);
            var parametersDeclare = ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.IntPtr, "__gchandle_ptr"));
            string constructorFullName = $"{@namespace.Replace("::", "__")}__{className}__{constructor.Name}__{i}__Injected";
            string callable = $"::Ayla::Object::ScriptNew<{classType.CppName}>";
            var codeGen = new FunctionBodyGenerator(parameters, callable, PlaceholderName.Value);

            m_Parent.WriteIndentedLine($"PLATFORM_SHARED_EXPORT ::Ayla::ObjectReferenceWrapper {constructorFullName}({parametersDeclare})");
            m_Parent.WriteIndentedLine("{");
            m_Parent.Indented(() =>
            {
                codeGen.GenerateCppCSharpToNative(m_Parent.WriteIndentedLine);
            });
            m_Parent.WriteIndentedLine("}");
        }
    }

    private void GenerateFunctionExports(string @namespace, string className, ClassName classType)
    {
        for (int i = 0; i < m_Class.Functions.Count; ++i)
        {
            var function = m_Class.Functions[i];
            var returnType = m_Parent.TypeNames.FindType(function.ReturnType, m_Class.Class);
            var parameters = CollectParameters(function.Parameters);

            string functionFullName = $"{@namespace.Replace("::", "__")}__{className}__{function.Name}__{i}__Injected";
            bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);
            bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);
            var caller = isStatic ? $"{classType.CppName}::" : $"(({classType.CppName}*)(::Ayla::Object*)self)->";
            string suffix = isVirtual ? "_Implementation" : string.Empty;
            string callable = $"{caller}{function.Name}{suffix}";

            FunctionBodyGenerator codeGen = new FunctionBodyGenerator(parameters, callable, returnType);
            string parametersDeclare = isStatic
                ? ParametersGenerator.GenerateCppBindings(parameters)
                : ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.IntPtr, "self"));

            m_Parent.WriteIndentedLine($"PLATFORM_SHARED_EXPORT {returnType.CppBindingName} {functionFullName}({parametersDeclare})");
            m_Parent.WriteIndentedLine("{");
            m_Parent.Indented(() =>
            {
                codeGen.GenerateCppCSharpToNative(m_Parent.WriteIndentedLine);
            });
            m_Parent.WriteIndentedLine("}");
        }
    }

    private ParameterCollection CollectParameters(IEnumerable<SParameter> parameters)
    {
        var collection = new ParameterCollection();
        foreach (var param in parameters)
        {
            var paramType = m_Parent.TypeNames.FindType(param.Variable.TypeName, m_Class.Class);
            collection.Add(paramType, param.Variable.Name);
        }
        return collection;
    }
}
