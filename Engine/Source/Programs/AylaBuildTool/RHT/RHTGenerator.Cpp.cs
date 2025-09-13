// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using AylaEngine.RHT.Syntaxes;
using AylaEngine.RHT.Types;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateSourceCode(ModuleProject project, TypeNames typeNames)
    {
        List<string> headers = [];
        foreach (var aclass in Classes)
        {
            foreach (var type in aclass.Properties.Select(p => p.Variable.TypeName)
                .Concat(aclass.Functions.Select(f => f.ReturnType)))
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

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is SAClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                string @namespace = string.Join("::", aclass.Class.Namespaces.Select(p => p.Name));
                string @class = aclass.Class.Name;
                var classType = typeNames.FindClass(aclass.Class);

                sourceCodeText += $"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {@class});\n";
                sourceCodeText += $"\n";

                if (@class == "Object")
                {
                    continue;
                }

                sourceCodeText += $"namespace {@namespace}\n";
                sourceCodeText +=  "{\n";
                sourceCodeText += $"  void {@class}::GatherProperties(::Ayla::PropertyCollector& collector)\n";
                sourceCodeText +=  "  {\n";
                sourceCodeText +=  "    Super::GatherProperties(collector);\n";
                sourceCodeText += $"    Transfer(collector);\n";
                sourceCodeText +=  "  }\n";
                for (int i = 0; i < aclass.Functions.Count; ++i)
                {
                    var function = aclass.Functions[i];
                    bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);
                    if (isVirtual == false)
                    {
                        continue;
                    }

                    var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                    sourceCodeText += $"  {function.ReturnType.FullName} {@class}::{function.Name}()\n";
                    sourceCodeText += $"  {{\n";
                    sourceCodeText += $"    using signature_t = {function.ReturnType.FullName}(*)(::Ayla::ObjectReferenceWrapper);\n";
                    sourceCodeText += $"    static auto callable = reinterpret_cast<signature_t>(::Ayla::ScriptingBackend::Get().GetFunctionPointer(\"{project.Name}.Script\", \"{classType.CSharpName["global::".Length..]}__Injected\", \"{function.Name}__Invoke\"));\n";
                    string callStatement = $"callable(AsWrapper())";
                    if (returnType == TypeName.Void)
                    {
                        sourceCodeText += $"    {callStatement};\n";
                    }
                    else
                    {
                        sourceCodeText += $"    return ::Ayla::Marshal::ToNative<{function.ReturnType.FullName}>({callStatement});\n";
                    }
                    sourceCodeText += $"  }}\n";
                }
                sourceCodeText +=  "}\n\n";
                sourceCodeText +=  "extern \"C\"\n";
                sourceCodeText +=  "{\n";
                for (int i = 0; i < aclass.Constructors.Count; ++i)
                {
                    var constructor = aclass.Constructors[i];
                    var parameterTypes = constructor.Parameters
                        .Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class))
                        .ToArray();
                    string parameters = string.Join(", ", parameterTypes.Select((p, i) =>
                    {
                        return $"{p.CppBindingName} {constructor.Parameters[i].Variable.Name}";
                    }));
                    string constructorFullName = $"{@namespace.Replace("::", "__")}__{@class}__{constructor.Name}__{i}__Injected";
                    string arguments = string.Join(", ", constructor.Parameters.Select(p =>
                    {
                        var argumentType = typeNames.FindType(p.Variable.TypeName, aclass.Class);
                        return $"::Ayla::Marshal::ToNative<{argumentType.CppName}>({p.Variable.Name})";
                    }));
                    string bodyStatement = $"::Ayla::Object::ScriptNew<{classType.CppName}>({arguments})";
                    string returnStatement = $"return {bodyStatement}->CreateLocker()";
                    sourceCodeText += $"  PLATFORM_SHARED_EXPORT ::Ayla::ObjectReferenceLocker {constructorFullName}({parameters})\n";
                    sourceCodeText += $"  {{\n";
                    sourceCodeText += $"    {returnStatement};\n";
                    sourceCodeText += $"  }}\n";
                }
                for (int i = 0; i < aclass.Functions.Count; ++i)
                {
                    var function = aclass.Functions[i];
                    var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                    var parameterTypes = function.Parameters
                        .Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class))
                        .ToArray();
                    string parameters = string.Join(", ", parameterTypes.Select((p, i) =>
                    {
                        return $"{p.CppBindingName} {function.Parameters[i].Variable.Name}";
                    }));
                    if (function.Flags.HasFlag(SFunction.FFlags.Static) == false)
                    {
                        if (string.IsNullOrEmpty(parameters))
                        {
                            parameters = "void* self";
                        }
                        else
                        {
                            parameters = string.Join(", ", "void* self", parameters);
                        }
                    }
                    string functionFullName = $"{@namespace.Replace("::", "__")}__{@class}__{function.Name}__{i}__Injected";
                    string arguments = string.Join(", ", function.Parameters.Select(p =>
                    {
                        var argumentType = typeNames.FindType(p.Variable.TypeName, aclass.Class);
                        return $"::Ayla::Marshal::ToNative<{argumentType.CppName}>({p.Variable.Name})";
                    }));
                    bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);
                    bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);
                    var caller = isStatic ? $"{classType.CppName}::" : $"(({classType.CppName}*)(::Ayla::Object*)self)->";
                    string suffix = isVirtual ? "_Implementation" : string.Empty;
                    string bodyStatement = $"{caller}{function.Name}{suffix}({arguments})";
                    string returnStatement;
                    if (returnType != BuiltinTypeName.Void)
                    {
                        returnStatement = $"return ::Ayla::Marshal::ToBinding({bodyStatement})";
                    }
                    else
                    {
                        returnStatement = $"{bodyStatement}";
                    }

                    sourceCodeText += $"  PLATFORM_SHARED_EXPORT {returnType.CppBindingName} {functionFullName}({parameters})\n";
                    sourceCodeText += $"  {{\n";
                    sourceCodeText += $"    {returnStatement};\n";
                    sourceCodeText += $"  }}\n";
                }
                sourceCodeText +=  "}\n";
                
                sourceCodeText +=  "\n";
            }
        }

        return sourceCodeText;
    }
}
