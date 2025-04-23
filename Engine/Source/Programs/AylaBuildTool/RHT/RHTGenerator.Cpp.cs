// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateSourceCode(RHTGenerator.Collection collection)
    {
        List<string> headers = [];
        foreach (var aclass in Classes)
        {
            foreach (var property in aclass.Properties)
            {
                if (collection.FindMatch(property.TypeName, aclass, out var generator, out _) == false)
                {
                    var context = property.Context;
                    throw new ParsingErrorException(context.FilePath, context.LineNumber, context.ColumnNumber, $"The requested class \"{property.TypeName.CSharp}\"'s defining header file could not be found in the Reflection header file list.");
                }

                headers.Add(generator.SourceCode.FilePath);
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
            if (syntax is AClass aclass)
            {
                var lineNumber = aclass.LineNumber;

                string @namespace = aclass.Class.NamespaceCpp;
                string @class = aclass.Class.Name;

                sourceCodeText += $"ACLASS__IMPL_CLASS_REGISTER({@namespace}, {@class});\n";
                sourceCodeText += "\n";
                sourceCodeText += "extern \"C\"\n";
                sourceCodeText += "{\n";
                sourceCodeText += $"\tPLATFORM_SHARED_EXPORT ::Ayla::ssize_t {FunctionName1("New")}()\n";
                sourceCodeText += "\t{\n";
                sourceCodeText += $"\t\tauto new_ = ::Ayla::Object::ScriptNew<::{@namespace}::{@class}>(0);\n";
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

                        if (function.Static == false)
                        {
                            parameters.Add("::Ayla::ssize_t self_");
                        }

                        foreach (var parameter in function.ParameterInfos)
                        {
                            parameters.Add($"{parameter.TypeName.CppBindingsParameter} {parameter.Name}");
                            arguments.Add(parameter.TypeName.CppBindingsArgument(aclass, parameter.Name));
                        }

                        sourceCodeText += $"\tPLATFORM_SHARED_EXPORT {function.Return.Cpp} {FunctionName1(function.Name)}({string.Join(", ", parameters)})\n";
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
