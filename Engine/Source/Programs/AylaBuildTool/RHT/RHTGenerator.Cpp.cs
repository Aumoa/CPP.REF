// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateSourceCode()
    {
        string sourceCodeText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#include "CoreMinimal.h"
#include "{m_SourceCode.FilePath.Replace('\\', '/')}"


""";

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is AClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                string @namespace = aclass.Class.Namespace;
                string @class = aclass.Class.Name;

                sourceCodeText += $"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {@class});\n";
                sourceCodeText += "\n";
                sourceCodeText += "extern \"C\"\n";
                sourceCodeText += "{\n";
                sourceCodeText += $"\tPLATFORM_SHARED_EXPORT ::Ayla::ssize_t {FunctionName1("New")}()\n";
                sourceCodeText += "\t{\n";
                sourceCodeText += $"\t\tauto new_ = ::Ayla::Object::UnsafeNew<::{@namespace}::{@class}>(0);\n";
                sourceCodeText += $"\t\tauto& self__ = *new ::Ayla::RPtr<::Ayla::Object>(new_);\n";
                sourceCodeText += $"\t\treturn ::Ayla::Marshal::RPtrToIntPtr(self__);\n";
                sourceCodeText += "\t}\n";
                sourceCodeText += "\n";
                sourceCodeText += $"\tPLATFORM_SHARED_EXPORT void {FunctionName1("Finalize")}(::Ayla::ssize_t self_)\n";
                sourceCodeText += "\t{\n";
                sourceCodeText += $"\t\tauto& self__ = ::Ayla::Marshal::IntPtrToRPtr(self_);\n";
                sourceCodeText += $"\t\tdelete &self__;\n";
                sourceCodeText += "\t}\n";
                sourceCodeText += "}\n";
                sourceCodeText += "\n";

                if (aclass.Body != null)
                {
                    sourceCodeText += $"namespace {@namespace}\n";
                    sourceCodeText += "{\n";
                    sourceCodeText += $"\tvoid {@class}::GatherProperties(::Ayla::PropertyCollector& collector)\n";
                    sourceCodeText += "\t{\n";
                    sourceCodeText += "\t\tSuper::GatherProperties(collector);\n";
                    foreach (var property in aclass.Properties)
                    {
                        sourceCodeText += $"\t\tGENERATED_BODY__GATHER_PROPERTIES_PROP({property.Name});\n";
                    }
                    sourceCodeText += "\t}\n";
                    sourceCodeText += "}\n\n";
                    sourceCodeText += "extern \"C\"\n";
                    sourceCodeText += "{\n";
                    foreach (var function in aclass.Functions)
                    {
                        List<string> parameters = [];
                        List<string> arguments = [];

                        IEnumerable<string> parametersWithSelf = parameters;
                        IEnumerable<string> argumentsWithSelf = arguments;

                        foreach (var parameter in function.ParameterInfos)
                        {
                            parameters.Add($"{FormatTypeToCpp(function.Context, parameter.TypeName)} {parameter.Name}");
                            arguments.Add(parameter.Name);
                        }

                        if (function.Static == false)
                        {
                            parameters.Add("::Ayla::ssize_t self_");
                        }

                        sourceCodeText += $"\tPLATFORM_SHARED_EXPORT {FormatReturnType1()} {FunctionName1(function.Name)}({string.Join(", ", parameters)})\n";
                        sourceCodeText += "\t{\n";
                        if (function.Static == false)
                        {
                            sourceCodeText += $"\t\tauto& self__ = {Self__()};\n";
                        }
                        sourceCodeText += $"\t\treturn {CallFunction()};\n";
                        sourceCodeText += "\t}\n";
                        sourceCodeText += "\n";

                        string CallFunction()
                        {
                            string prefix;
                            if (function.Static)
                            {
                                prefix = $"::{@namespace}::{@class}::";
                            }
                            else
                            {
                                prefix = $"self__->";
                            }

                            return $"{prefix}{function.Name}(" + string.Join(", ", arguments) + ")";
                        }

                        string FormatReturnType1() => FormatTypeToCpp(function.Context, function.Return);
                    }
                    sourceCodeText += "}\n";
                    sourceCodeText += "\n";
                }

                string FunctionName1(string name) => FunctionName(@namespace, @class, name);

                string Self__()
                {
                    return $"(::Ayla::Marshal::IntPtrToRPtr<::{@namespace}::{@class}>(self_))";
                }
            }
        }

        return sourceCodeText;
    }
}
