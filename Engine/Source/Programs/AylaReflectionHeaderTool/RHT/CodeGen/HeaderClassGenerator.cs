// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C++ class header definitions.
/// </summary>
internal class HeaderClassGenerator
{
    private readonly HeaderGenerator m_Parent;
    private readonly SAClass m_Class;

    public HeaderClassGenerator(HeaderGenerator parent, SAClass aclass)
    {
        m_Parent = parent;
        m_Class = aclass;
    }

    public void Generate()
    {
        var lineNumber = m_Class.Body.LineNumber;
        var scope = string.Join("::", m_Class.Class.Namespaces.Select(ns => ns.Name));
        string @namespace = string.Join("::", m_Class.Class.Namespaces.Select(p => p.Name));
        string className = m_Class.Class.Name;
        var @class = m_Parent.TypeNames.FindClass(m_Class.Class);

        GenerateExternCBlock(@namespace, className, @class);
        GenerateReflexprClassMacro(lineNumber, scope, className, @class);
        GenerateScriptTableMacro(lineNumber, @namespace, className);
        GenerateGeneratedBodyMacro(lineNumber, className);
    }

    private void GenerateExternCBlock(string @namespace, string className, ClassName @class)
    {
        m_Parent.HeaderText += m_Parent.IndentedLine($"extern \"C\"");
        m_Parent.HeaderText += m_Parent.IndentedLine("{");
        
        List<string> friends = [];
        m_Parent.Indented(() =>
        {
            for (int i = 0; i < m_Class.Functions.Count; ++i)
            {
                var function = m_Class.Functions[i];
                var returnType = m_Parent.TypeNames.FindType(function.ReturnType, m_Class.Class);
                var parameters = CollectParameters(function.Parameters);
                string functionFullName = $"{@namespace.Replace("::", "__")}__{className}__{function.Name}__{i}__Injected";
                string paramsDeclare = function.Flags.HasFlag(SFunction.FFlags.Static)
                    ? ParametersGenerator.GenerateCppBindings(parameters)
                    : ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.IntPtr, "self"));

                m_Parent.HeaderText += m_Parent.IndentedLine($"PLATFORM_SHARED_EXPORT {returnType.CppBindingName} {functionFullName}({paramsDeclare});");
                friends.Add($"friend {returnType.CppBindingName} (::{functionFullName})({paramsDeclare});");
            }
        });
        m_Parent.HeaderText += m_Parent.IndentedLine("}");
    }

    private void GenerateReflexprClassMacro(int lineNumber, string scope, string className, ClassName @class)
    {
        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"#define GENERATED_BODY__IMPL__{m_Parent.FileId}__{lineNumber}__reflexpr_class ");
        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"struct reflexpr_class");
        m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
        m_Parent.Indented(() =>
        {
            GenerateReflexprAliased(scope, className);
            GenerateReflexprMembers(className);
        });
        m_Parent.HeaderText += m_Parent.IndentedLine("};");
        m_Parent.HeaderText += m_Parent.IndentedLine("");
    }

    private void GenerateReflexprAliased(string scope, string className)
    {
        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"struct aliased : public ::std::experimental::reflect::reflexpr_aliased");
        m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
        m_Parent.Indented(() =>
        {
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static constexpr ::Ayla::String name = TEXT(\"{className}\");");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static constexpr ::Ayla::String scope = TEXT(\"{scope}\");");
        });
        m_Parent.HeaderText += m_Parent.IndentedMacroLine("};");
        m_Parent.HeaderText += m_Parent.IndentedMacroLine("");
    }

    private void GenerateReflexprMembers(string className)
    {
        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"struct members : public ::std::experimental::reflect::reflexpr_members");
        m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
        
        GenerateReflexprFields(className);
        GenerateReflexprConstructors(className);
        GenerateReflexprFunctions(className);
        GenerateReflexprMembersGetMethod();
        
        m_Parent.HeaderText += m_Parent.IndentedMacroLine("};");
    }

    private void GenerateReflexprFields(string className)
    {
        foreach (var field in m_Class.Properties)
        {
            var fieldType = m_Parent.TypeNames.FindType(field.Variable.TypeName, m_Class.Class);
            var access = $"::std::experimental::reflect::access_type::{field.Access.ToString().ToLower()}_";
            
            m_Parent.Indented(() =>
            {
                m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static consteval auto {field.Variable.Name}()");
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
                m_Parent.Indented(() =>
                {
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"/*{fieldType.Id} {field.Variable.Name}*/");
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"return ::std::experimental::reflect::reflexpr_field<{access}, decltype({className}::{field.Variable.Name}), offsetof({className}, {field.Variable.Name})>();");
                });
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("}");
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("");
            });
        }
    }

    private void GenerateReflexprConstructors(string className)
    {
        for (int i = 0; i < m_Class.Constructors.Count; ++i)
        {
            var constructor = m_Class.Constructors[i];
            var access = $"::std::experimental::reflect::access_type::{constructor.Access.ToString().ToLower()}_";
            
            m_Parent.Indented(() =>
            {
                var parameterDeclare1 = string.Join(", ", constructor.Parameters.Select(p => p.Variable.TypeName.FullName + " " + p.Variable.Name));
                m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static ::Ayla::SharedPtr<::Ayla::Object> constructor__{i}__{constructor.Name}({parameterDeclare1});");
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("");
                m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static consteval auto {constructor.Name}__{i}()");
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
                m_Parent.Indented(() =>
                {
                    var parameterDeclare2 = string.Join(", ", constructor.Parameters.Select(p => p.Variable.TypeName.FullName));
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"using signature_t = ::Ayla::SharedPtr<::Ayla::Object>(*)({parameterDeclare2});");
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"constexpr auto ptr = (signature_t)&constructor__{i}__{constructor.Name};");
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"return ::std::experimental::reflect::reflexpr_constructor<{access}, signature_t, ptr>();");
                });
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("}");
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("");
            });
        }
    }

    private void GenerateReflexprFunctions(string className)
    {
        for (int i = 0; i < m_Class.Functions.Count; ++i)
        {
            var function = m_Class.Functions[i];
            var access = $"::std::experimental::reflect::access_type::{function.Access.ToString().ToLower()}_";
            var returnType = m_Parent.TypeNames.FindType(function.ReturnType, m_Class.Class);
            
            m_Parent.Indented(() =>
            {
                m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static consteval auto {function.Name}__{i}()");
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
                m_Parent.Indented(() =>
                {
                    var owned = function.Flags.HasFlag(SFunction.FFlags.Static) ? string.Empty : $"{className}::";
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"using signature_t = {function.ReturnType.FullName}({owned}*)({string.Join(", ", function.Parameters.Select(p => p.Variable.TypeName.FullName))});");
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"constexpr auto ptr = (signature_t)&{className}::{function.Name};");
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"return ::std::experimental::reflect::reflexpr_method<{access}, signature_t, ptr>();");
                });
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("}");
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("");
            });
        }
    }

    private void GenerateReflexprMembersGetMethod()
    {
        int count = 0;

        m_Parent.Indented(() =>
        {
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"template<size_t N>");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static consteval auto get() noexcept");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
            
            for (int i = 0; i < m_Class.Properties.Count; ++i)
            {
                m_Parent.Indented(() =>
                {
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"if constexpr (N == {count++})");
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
                    m_Parent.Indented(() =>
                    {
                        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"return {m_Class.Properties[i].Variable.Name}();");
                    });
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine("}");
                });
            }
            
            for (int i = 0; i < m_Class.Constructors.Count; ++i)
            {
                m_Parent.Indented(() =>
                {
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"if constexpr (N == {count++})");
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
                    m_Parent.Indented(() =>
                    {
                        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"return {m_Class.Constructors[i].Name}__{i}();");
                    });
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine("}");
                });
            }
            
            for (int i = 0; i < m_Class.Functions.Count; ++i)
            {
                m_Parent.Indented(() =>
                {
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"if constexpr (N == {count++})");
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
                    m_Parent.Indented(() =>
                    {
                        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"return {m_Class.Functions[i].Name}__{i}();");
                    });
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine("}");
                });
            }
            
            if (count > 0)
            {
                m_Parent.Indented(() =>
                {
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"else");
                });
            }
            
            m_Parent.Indented(() =>
            {
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("{");
                m_Parent.Indented(() =>
                {
                    m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static_assert(N < {count}, \"Invalid reflexpr index\");");
                });
                m_Parent.HeaderText += m_Parent.IndentedMacroLine("}");
            });
            
            m_Parent.HeaderText += m_Parent.IndentedMacroLine("}");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine("");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"static constexpr size_t tuple_size = {count};");
        });
    }

    private void GenerateScriptTableMacro(int lineNumber, string @namespace, string className)
    {
        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"#define GENERATED_BODY__IMPL__{m_Parent.FileId}__{lineNumber}__script_table ");
        m_Parent.Indented(() =>
        {
            for (int i = 0; i < m_Class.Functions.Count; ++i)
            {
                var function = m_Class.Functions[i];
                if (!function.Flags.HasFlag(SFunction.FFlags.Virtual))
                {
                    continue;
                }

                var @params = string.Join(", ", function.Parameters.Select(p => $"{p.Variable.TypeName.FullName} {p.Variable.Name}"));
                var suffix = function.Flags.HasFlag(SFunction.FFlags.Const) ? " const" : string.Empty;
                suffix += function.Flags.HasFlag(SFunction.FFlags.Pure) ? " = 0" : string.Empty;
                m_Parent.HeaderText += m_Parent.IndentedMacroLine($"virtual {function.ReturnType.FullName} {function.Name}_Implementation({@params}){suffix};");
            }
        });
        m_Parent.HeaderText += m_Parent.IndentedLine("");
    }

    private void GenerateGeneratedBodyMacro(int lineNumber, string className)
    {
        m_Parent.HeaderText += m_Parent.IndentedMacroLine($"#define GENERATED_BODY__IMPL__{m_Parent.FileId}__{lineNumber} ");
        m_Parent.Indented(() =>
        {
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"GENERATED_BODY__DEFAULT_BODY({className}) ");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"GENERATED_BODY__IMPL__{m_Parent.FileId}__{lineNumber}__reflexpr_class");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"GENERATED_BODY__IMPL__{m_Parent.FileId}__{lineNumber}__script_table");
            
            // Add friend declarations
            var @class = m_Parent.TypeNames.FindClass(m_Class.Class);
            string @namespace = string.Join("::", m_Class.Class.Namespaces.Select(p => p.Name));
            for (int i = 0; i < m_Class.Functions.Count; ++i)
            {
                var function = m_Class.Functions[i];
                var returnType = m_Parent.TypeNames.FindType(function.ReturnType, m_Class.Class);
                var parameters = CollectParameters(function.Parameters);
                string functionFullName = $"{@namespace.Replace("::", "__")}__{className}__{function.Name}__{i}__Injected";
                string paramsDeclare = function.Flags.HasFlag(SFunction.FFlags.Static)
                    ? ParametersGenerator.GenerateCppBindings(parameters)
                    : ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.IntPtr, "self"));
                
                m_Parent.HeaderText += m_Parent.IndentedMacroLine($"friend {returnType.CppBindingName} (::{functionFullName})({paramsDeclare});");
            }
            
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"GENERATED_BODY__DECLARE_GATHER_PROPERTIES()");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"GENERATED_BODY__DECLARE_TRANSFER()");
            m_Parent.HeaderText += m_Parent.IndentedMacroLine($"GENERATED_BODY__DECLARE_MANAGED_TYPE()");
        });
        m_Parent.HeaderText += m_Parent.IndentedLine("private:\n");
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
