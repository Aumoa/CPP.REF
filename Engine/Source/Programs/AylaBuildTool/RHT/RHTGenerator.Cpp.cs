// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Reflection.Metadata;
using System.Security.Cryptography;
using AylaEngine.RHT.Syntaxes;
using AylaEngine.RHT.Types;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateSourceCode(TypeNames typeNames)
    {
        List<string> headers = [];
        foreach (var aclass in Classes)
        {
            foreach (var property in aclass.Properties)
            {
                var type = typeNames.FindType(property.Variable.TypeName, aclass.Class);
                if (type is ClassName className)
                {
                    headers.Add(className.Source.SourceCode.FilePath);
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
                sourceCodeText +=  "}\n\n";
                sourceCodeText +=  "extern \"C\"\n";
                sourceCodeText +=  "{\n";
                for (int i = 0; i < aclass.Functions.Count; ++i)
                {
                    var function = aclass.Functions[i];
                    var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                    var parameterTypes = function.Parameters
                        .Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class))
                        .ToArray();
                    string parameters = string.Join(", ", parameterTypes.Select((p, i) =>
                    {
                        return $"{p.BindingName} {function.Parameters[i].Variable.Name}";
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
                    string arguments = string.Join(", ", function.Parameters.Select(p => p.Variable.Name));
                    string bodyStatement = $"(({classType.CppName}*)(::Ayla::Object*)self)->{function.Name}({arguments})";
                    string returnStatement;
                    if (returnType != BuiltinTypeName.Void)
                    {
                        returnStatement = $"return ::Ayla::Marshal::MarshalToBinding({bodyStatement})";
                    }
                    else
                    {
                        returnStatement = $"{bodyStatement}";
                    }
                    sourceCodeText += $"  PLATFORM_SHARED_EXPORT {returnType.BindingName} {functionFullName}({parameters})\n";
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
