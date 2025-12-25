// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using AylaEngine.RHT.CodeGen;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateSourceCode(ModuleProject project, TargetInfo buildTarget, TypeNames typeNames)
    {
        List<string> headers = [];
        var rule = project.GetRule(buildTarget);
        foreach (var aclass in Classes)
        {
            foreach (var type in aclass
                .Properties.Select(p => p.Variable.TypeName)
                .Concat(aclass.Functions.Select(f => f.ReturnType))
                .Concat(aclass.Functions.SelectMany(f => f.Parameters.Select(p => p.Variable.TypeName)))
                .Concat(aclass.Constructors.SelectMany(f => f.Parameters.Select(p => p.Variable.TypeName))))
            {
                var typeName = typeNames.FindType(type, aclass.Class);
                HandleElementType(typeName);

                continue;

                void HandleElementType(TypeName typeName)
                {
                    if (typeName is ClassName className)
                    {
                        headers.Add(className.Source.SourceCode.FilePath);
                    }
                    else if (typeName is SharedPtrTypeName rptr)
                    {
                        headers.Add(((ClassName)rptr.ElementType).Source.SourceCode.FilePath);
                    }
                    else if (typeName is ArrayTypeName array)
                    {
                        HandleElementType(array.ElementType);
                    }
                }
            }
        }

        string headersInclude = string.Empty;
        if (headers.Count > 0)
        {
            headersInclude = "\n" + string.Join("\n", headers.Select(header => $"#include \"{header.Replace('\\', '/')}\""));
        }

        string sourceCodeText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#include "CoreMinimal.h"
#include "Reflection/TypeCollector.h"{headersInclude}
#include "{SourceCode.FilePath.Replace('\\', '/')}"


""";

        int indent = 0;

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is SAClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                string @namespace = string.Join("::", aclass.Class.Namespaces.Select(p => p.Name));
                string className = aclass.Class.Name;
                var classType = typeNames.FindClass(aclass.Class);

                WriteIndentedLine($"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {className});");
                WriteIndentedLine($"");

                if (className == "Object")
                {
                    continue;
                }

                WriteIndentedLine($"namespace {@namespace}");
                WriteIndentedLine($"{{");
                Indented(() =>
                {
                    for (int i = 0; i < aclass.Constructors.Count; ++i)
                    {
                        var constructor = aclass.Constructors[i];
                        var parameterDeclare1 = string.Join(", ", constructor.Parameters.Select(p => p.Variable.TypeName.FullName + " " + p.Variable.Name));
                        WriteIndentedLine($"::Ayla::SharedPtr<::Ayla::Object> {className}::reflexpr_class::members::constructor__{i}__{constructor.Name}({parameterDeclare1})");
                        WriteIndentedLine($"{{");
                        Indented(() =>
                        {
                            var arguments = string.Join(", ", constructor.Parameters.Select(p => p.Variable.Name));
                            WriteIndentedLine($"return ::Ayla::Object::UnsafeNew<{aclass.Class.Name}>({arguments});");
                        });
                        WriteIndentedLine($"}}");
                    }

                    WriteIndentedLine($"");
                    WriteIndentedLine($"void {className}::GatherProperties(::Ayla::PropertyCollector& collector)");
                    WriteIndentedLine($"{{");
                    Indented(() =>
                    {
                        WriteIndentedLine($"Super::GatherProperties(collector);");
                        WriteIndentedLine($"Transfer(collector);");
                    });
                    WriteIndentedLine($"}}");
                    WriteIndentedLine($"");
                    WriteIndentedLine($"::Ayla::ManagedTypeWrapper {className}::GetManagedType()");
                    WriteIndentedLine($"{{");
                    Indented(() =>
                    {
                        WriteIndentedLine($"using signature_t = void*(*)();");
                        WriteIndentedLine($"static ::Ayla::ManagedTypeWrapper s_Type =");
                        WriteIndentedLine($"{{");
                        Indented(() =>
                        {
                            WriteIndentedLine($".NativeType = ::Ayla::TypeCollector::FindType(typeid({className})),");
                            WriteIndentedLine($".ScriptTypeGetter = reinterpret_cast<signature_t>(::Ayla::ScriptingBackend::Get().GetFunctionPointer(\"{project.Name}.Script\", \"{classType.CSharpName["global::".Length..]}__Invocable\", \"GetScriptType__Invoke\"))()");
                        });
                        WriteIndentedLine($"}};");
                        WriteIndentedLine($"");
                        WriteIndentedLine($"return s_Type;");
                    });
                    WriteIndentedLine($"}}");
                    WriteIndentedLine($"");

                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                        var function = aclass.Functions[i];
                        bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);
                        if (isVirtual == false)
                        {
                            continue;
                        }

                        var parameters = new ParameterCollection();
                        foreach (var param in function.Parameters)
                        {
                            var paramType = typeNames.FindType(param.Variable.TypeName, aclass.Class);
                            parameters.Add(paramType, param.Variable.Name);
                        }

                        var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                        var parametersDeclare = ParametersGenerator.GenerateCpp(parameters);
                        var suffix = function.Flags.HasFlag(SFunction.FFlags.Const) ? " const" : string.Empty;
                        WriteIndentedLine($"{function.ReturnType.FullName} {className}::{function.Name}({parametersDeclare}){suffix}");
                        WriteIndentedLine($"{{");
                        Indented(() =>
                        {
                            var invokeParametersDeclare = ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.Object, "self_"));
                            WriteIndentedLine($"using signature_t = {returnType.CppBindingName}(*)({invokeParametersDeclare});");
                            WriteIndentedLine($"static auto callable = reinterpret_cast<signature_t>(::Ayla::ScriptingBackend::Get().GetFunctionPointer(\"{project.Name}.Script\", \"{classType.CSharpName["global::".Length..]}__Invocable\", \"{function.Name}__Invoke\"));");
                            WriteIndentedLine($"auto self = AsShared();");
                            string callable = $"callable";
                            var codeGen = new FunctionBodyGenerator(parameters.AddFirstTemp(SharedPtrTypeName.SharedObject, "self"), callable, returnType);
                            codeGen.GenerateCppNativeToCSharp(WriteIndentedLine);
                        });
                        WriteIndentedLine($"}}");
                        if (rule.Type == ModuleType.Application || rule.Type == ModuleType.Console)
                        {
                            WriteIndentedLine($"{function.ReturnType.FullName} {className}::{function.Name}_Implementation({parametersDeclare})");
                            WriteIndentedLine($"{{");
                            Indented(() =>
                            {
                                WriteIndentedLine($"throw ::Ayla::AccessViolationException(TEXT(\"Assemblies of the Application or Console type cannot directly invoke native functions.\"));");
                            });
                            WriteIndentedLine($"}}");
                        }
                    }
                });
                WriteIndentedLine($"}}");
                WriteIndentedLine($"");
                WriteIndentedLine($"extern \"C\"");
                WriteIndentedLine($"{{");
                Indented(() =>
                {
                    WriteIndentedLine($"PLATFORM_SHARED_EXPORT ::Ayla::ManagedTypeWrapper {classType.CppName[2..].Replace("::", "__")}__GetManagedType()");
                    WriteIndentedLine($"{{");
                    Indented(() =>
                    {
                        WriteIndentedLine($"return {@namespace}::{className}::GetManagedType();");
                    });
                    WriteIndentedLine($"}}");
                    WriteIndentedLine($"");

                    for (int i = 0; i < aclass.Constructors.Count; ++i)
                    {
                        var constructor = aclass.Constructors[i];
                        var parameters = new ParameterCollection();
                        foreach (var param in constructor.Parameters)
                        {
                            var paramType = typeNames.FindType(param.Variable.TypeName, aclass.Class);
                            parameters.Add(paramType, param.Variable.Name);
                        }
                        var parametersDeclare = ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.IntPtr, "__gchandle_ptr"));
                        string constructorFullName = $"{@namespace.Replace("::", "__")}__{className}__{constructor.Name}__{i}__Injected";
                        string arguments = string.Join(", ", constructor.Parameters.Select(p =>
                        {
                            var argumentType = typeNames.FindType(p.Variable.TypeName, aclass.Class);
                            return $"::Ayla::Marshal::ToNative<{argumentType.CppName}>({p.Variable.Name})";
                        }));
                        string callable = $"::Ayla::Object::ScriptNew<{classType.CppName}>";
                        var codeGen = new FunctionBodyGenerator(parameters, callable, PlaceholderName.Value);
                        WriteIndentedLine($"PLATFORM_SHARED_EXPORT void* {constructorFullName}({parametersDeclare})");
                        WriteIndentedLine($"{{");
                        Indented(() =>
                        {
                            codeGen.GenerateCppCSharpToNative(WriteIndentedLine);
                        });
                        WriteIndentedLine($"}}");
                    }
                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                        var function = aclass.Functions[i];
                        var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                        var parameters = new ParameterCollection();
                        foreach (var param in function.Parameters)
                        {
                            var paramType = typeNames.FindType(param.Variable.TypeName, aclass.Class);
                            parameters.Add(paramType, param.Variable.Name);
                        }

                        string functionFullName = $"{@namespace.Replace("::", "__")}__{className}__{function.Name}__{i}__Injected";
                        bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);
                        bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);
                        var caller = isStatic ? $"{classType.CppName}::" : $"(({classType.CppName}*)(::Ayla::Object*)self)->";
                        string suffix = isVirtual ? "_Implementation" : string.Empty;
                        string callable = $"{caller}{function.Name}{suffix}";
                        FunctionBodyGenerator codeGen = new FunctionBodyGenerator(parameters, callable, returnType);
                        string parametersDeclare = ParametersGenerator.GenerateCppBindings(parameters);
                        if (function.Flags.HasFlag(SFunction.FFlags.Static) == false)
                        {
                            parametersDeclare = ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.IntPtr, "self"));
                        }
                        else
                        {
                            parametersDeclare = ParametersGenerator.GenerateCppBindings(parameters);
                        }
                        WriteIndentedLine($"PLATFORM_SHARED_EXPORT {returnType.CppBindingName} {functionFullName}({parametersDeclare})");
                        WriteIndentedLine($"{{");
                        Indented(() =>
                        {
                            codeGen.GenerateCppCSharpToNative(WriteIndentedLine);
                        });
                        WriteIndentedLine($"}}");
                    }
                });
                WriteIndentedLine($"}}");
                WriteIndentedLine($"");
            }

            if (syntax is SAEnum aenum)
            {
                string cppnamespace = string.Join("__", aenum.Namespaces.Select(n => n.Name));
                string fullname = "::" + string.Join("::", aenum.Namespaces.Select(p => p.Name).Append(aenum.Name));
                string functionName = $"{cppnamespace.Replace("::", "__")}__{aenum.Name}__ToString";

                WriteIndentedLine($"PLATFORM_SHARED_EXPORT ::Ayla::String {functionName}({fullname} value)");
                WriteIndentedLine($"{{");
                Indented(() =>
                {
                    WriteIndentedLine($"switch (value)");
                    WriteIndentedLine($"{{");
                    Indented(() =>
                    {
                        foreach (var define in aenum.Defines)
                        {
                            WriteIndentedLine($"case {fullname}::{define.Name}:");
                            Indented(() =>
                            {
                                WriteIndentedLine($"return TEXT(\"{define.Name}\");");
                            });
                        }

                        WriteIndentedLine($"default:");
                        Indented(() =>
                        {
                            WriteIndentedLine($"return ::Ayla::String::Format(TEXT(\"{aenum.Name}({{}})\"), (int)value);");
                        });
                    });
                    WriteIndentedLine($"}}");
                });
                WriteIndentedLine($"}}");
            }
        }

        return sourceCodeText;

        void Indented(Action body)
        {
            ++indent;
            try
            {
                body();
            }
            finally
            {
                --indent;
            }
        }

        void WriteIndentedLine(string text)
        {
            sourceCodeText += new string(' ', indent * 2) + text + "\n";
        }
    }
}
