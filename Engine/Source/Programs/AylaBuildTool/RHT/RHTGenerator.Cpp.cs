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
                .Concat(aclass.Functions.SelectMany(f => f.Parameters.Select(p => p.Variable.TypeName).Append(f.ReturnType))))
            {
                var typeName = typeNames.FindType(type, aclass.Class);
                if (typeName is ClassName className)
                {
                    headers.Add(className.Source.SourceCode.FilePath);
                }
                else if (typeName is SharedPtrTypeName rptr)
                {
                    headers.Add(((ClassName)rptr.ElementType).Source.SourceCode.FilePath);
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

#include "CoreMinimal.h"{headersInclude}
#include "{SourceCode.FilePath.Replace('\\', '/')}"


""";

        int indent = 0;

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is SAClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                string @namespace = string.Join("::", aclass.Class.Namespaces.Select(p => p.Name));
                string @class = aclass.Class.Name;
                var classType = typeNames.FindClass(aclass.Class);

                WriteIndentedLine($"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {@class});");
                WriteIndentedLine($"");

                if (@class == "Object")
                {
                    continue;
                }

                WriteIndentedLine($"namespace {@namespace}");
                WriteIndentedLine($"{{");
                Indented(() =>
                {
                    WriteIndentedLine($"void {@class}::GatherProperties(::Ayla::PropertyCollector& collector)");
                    WriteIndentedLine($"  {{");
                    Indented(() =>
                    {
                        WriteIndentedLine($"Super::GatherProperties(collector);");
                        WriteIndentedLine($"Transfer(collector);");
                    });
                    WriteIndentedLine($"}}");

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
                        WriteIndentedLine($"{function.ReturnType.FullName} {@class}::{function.Name}({parametersDeclare})");
                        WriteIndentedLine($"{{\n");
                        Indented(() =>
                        {
                            var invokeParametersDeclare = ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.Object, "self_"));
                            WriteIndentedLine($"using signature_t = {returnType.CppBindingName}(*)({invokeParametersDeclare});");
                            WriteIndentedLine($"static auto callable = reinterpret_cast<signature_t>(::Ayla::ScriptingBackend::Get().GetFunctionPointer(\"{project.Name}.Script\", \"{classType.CSharpName["global::".Length..]}__Injected\", \"{function.Name}__Invoke\"));");
                            WriteIndentedLine($"auto self = SharedFromThis();");
                            string callable = $"callable";
                            var codeGen = new FunctionBodyGenerator(parameters.AddFirstTemp(SharedPtrTypeName.SharedObject, "self"), callable, returnType);
                            codeGen.GenerateCppNativeToCSharp(WriteIndentedLine);
                        });
                        WriteIndentedLine($"}}");
                        if (rule.Type == ModuleType.Application)
                        {
                            WriteIndentedLine($"{function.ReturnType.FullName} {@class}::{function.Name}_Implementation({parametersDeclare})");
                            WriteIndentedLine($"{{");
                            Indented(() =>
                            {
                                WriteIndentedLine($"throw ::Ayla::AccessViolationException(TEXT(\"Assemblies of the Application type cannot directly invoke native functions.\"));");
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
                    for (int i = 0; i < aclass.Constructors.Count; ++i)
                    {
                        var constructor = aclass.Constructors[i];
                        var parameters = new ParameterCollection();
                        foreach (var param in constructor.Parameters)
                        {
                            var paramType = typeNames.FindType(param.Variable.TypeName, aclass.Class);
                            parameters.Add(paramType, param.Variable.Name);
                        }
                        var parametersDeclare = ParametersGenerator.GenerateCppBindings(parameters);
                        string constructorFullName = $"{@namespace.Replace("::", "__")}__{@class}__{constructor.Name}__{i}__Injected";
                        string arguments = string.Join(", ", constructor.Parameters.Select(p =>
                        {
                            var argumentType = typeNames.FindType(p.Variable.TypeName, aclass.Class);
                            return $"::Ayla::Marshal::ToNative<{argumentType.CppName}>({p.Variable.Name})";
                        }));
                        string callable = $"::Ayla::Object::ScriptNew<{classType.CppName}>";
                        var codeGen = new FunctionBodyGenerator(parameters, callable, PlaceholderName.Value);
                        string bodyStatement = $"::Ayla::Object::ScriptNew<{classType.CppName}>({arguments})";
                        string returnStatement = $"return {bodyStatement}";
                        WriteIndentedLine($"PLATFORM_SHARED_EXPORT ::Ayla::ObjectReferenceLocker {constructorFullName}({parametersDeclare})");
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

                        string functionFullName = $"{@namespace.Replace("::", "__")}__{@class}__{function.Name}__{i}__Injected";
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
