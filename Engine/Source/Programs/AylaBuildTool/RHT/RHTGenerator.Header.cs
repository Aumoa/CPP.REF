// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using AylaEngine.RHT.CodeGen;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateHeader(TypeNames typeNames)
    {
        string headerText = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#pragma once

#include "CoreMinimal.h"

#undef GENERATED_BODY__FILE_ID__
#define GENERATED_BODY__FILE_ID__ {m_FileId}


""";

        int indent = 0;

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is SAClass aclass)
            {
                if (aclass.Class.Name == "Object")
                {
                    continue;
                }

                var lineNumber = aclass.Body.LineNumber;
                var scope = string.Join("::", aclass.Class.Namespaces.Select(ns => ns.Name));
                string @namespace = string.Join("::", aclass.Class.Namespaces.Select(p => p.Name));
                string className = aclass.Class.Name;
                var @class = typeNames.FindClass(aclass.Class);

                headerText += Indented_Line($"extern \"C\"");
                headerText += Indented_Line($"{{");
                List<string> friends = [];
                Indented(() =>
                {
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
                        string paramsDeclare;
                        if (function.Flags.HasFlag(SFunction.FFlags.Static) == false)
                        {
                            paramsDeclare = ParametersGenerator.GenerateCppBindings(parameters.AddFirstTemp(TypeName.IntPtr, "self"));
                        }
                        else
                        {
                            paramsDeclare = ParametersGenerator.GenerateCppBindings(parameters);
                        }
                        headerText += Indented_Line($"PLATFORM_SHARED_EXPORT {returnType.CppBindingName} {functionFullName}({paramsDeclare});");
                        friends.Add($"friend {returnType.CppBindingName} (::{functionFullName})({paramsDeclare});");
                    }
                });
                headerText += Indented_Line($"}}");

                headerText += IndentedMLine($"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber}__reflexpr_class ");
                headerText += IndentedMLine($"struct reflexpr_class");
                headerText += IndentedMLine($"{{");
                Indented(() =>
                {
                    headerText += IndentedMLine($"struct aliased : public ::std::experimental::reflect::reflexpr_aliased");
                    headerText += IndentedMLine($"{{");
                    Indented(() =>
                    {
                        headerText += IndentedMLine($"static constexpr ::Ayla::String name = TEXT(\"{aclass.Class.Name}\");");
                        headerText += IndentedMLine($"static constexpr ::Ayla::String scope = TEXT(\"{scope}\");");
                    });
                    headerText += IndentedMLine($"}};");
                    headerText += IndentedMLine($"");
                    headerText += IndentedMLine($"struct members : public ::std::experimental::reflect::reflexpr_members");
                    headerText += IndentedMLine($"{{");
                    foreach (var field in aclass.Properties)
                    {
                        var fieldType = typeNames.FindType(field.Variable.TypeName, aclass.Class);
                        var access = $"::std::experimental::reflect::access_type::{field.Access.ToString().ToLower()}_";
                        Indented(() =>
                        {
                            headerText += IndentedMLine($"static consteval auto {field.Variable.Name}()");
                            headerText += IndentedMLine($"{{");
                            Indented(() =>
                            {
                                headerText += IndentedMLine($"/*{fieldType.Id} {field.Variable.Name}*/");
                                headerText += IndentedMLine($"return ::std::experimental::reflect::reflexpr_field<{access}, decltype({aclass.Class.Name}::{field.Variable.Name}), offsetof({aclass.Class.Name}, {field.Variable.Name})>();");
                            });
                            headerText += IndentedMLine($"}}");
                            headerText += IndentedMLine($"");
                        });
                    }
                    for (int i = 0; i < aclass.Constructors.Count; ++i)
                    {
                        var constructor = aclass.Constructors[i];
                        var access = $"::std::experimental::reflect::access_type::{constructor.Access.ToString().ToLower()}_";
                        var returnType = TypeName.Object;
                        Indented(() =>
                        {
                            var parameterDeclare1 = string.Join(", ", constructor.Parameters.Select(p => p.Variable.TypeName.FullName + " " + p.Variable.Name));
                            headerText += IndentedMLine($"static ::Ayla::SharedPtr<::Ayla::Object> constructor__{i}__{constructor.Name}({parameterDeclare1});");
                            headerText += IndentedMLine($"");
                            headerText += IndentedMLine($"static consteval auto {constructor.Name}__{i}()");
                            headerText += IndentedMLine($"{{");
                            Indented(() =>
                            {
                                var parameterDeclare2 = string.Join(", ", constructor.Parameters.Select(p => p.Variable.TypeName.FullName));
                                headerText += IndentedMLine($"using signature_t = ::Ayla::SharedPtr<::Ayla::Object>(*)({parameterDeclare2});");
                                headerText += IndentedMLine($"return ::std::experimental::reflect::reflexpr_constructor<{access}, signature_t, (signature_t)&constructor__{i}__{constructor.Name}>();");
                            });
                            headerText += IndentedMLine($"}}");
                            headerText += IndentedMLine($"");
                        });
                    }
                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                        var function = aclass.Functions[i];
                        var access = $"::std::experimental::reflect::access_type::{function.Access.ToString().ToLower()}_";
                        var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                        Indented(() =>
                        {
                            headerText += IndentedMLine($"static consteval auto {function.Name}__{i}()");
                            headerText += IndentedMLine($"{{");
                            Indented(() =>
                            {
                                var owned = function.Flags.HasFlag(SFunction.FFlags.Static) ? string.Empty : $"{aclass.Class.Name}::";
                                headerText += IndentedMLine($"using signature_t = {function.ReturnType.FullName}({owned}*)({string.Join(", ", function.Parameters.Select(p => p.Variable.TypeName.FullName))});");
                                headerText += IndentedMLine($"return ::std::experimental::reflect::reflexpr_method<{access}, signature_t, (signature_t)&{aclass.Class.Name}::{function.Name}>();");
                            });
                            headerText += IndentedMLine($"}}");
                            headerText += IndentedMLine($"");
                        });
                    }
                    Indented(() =>
                    {
                        int count = 0;

                        headerText += IndentedMLine($"template<size_t N>");
                        headerText += IndentedMLine($"static consteval auto get() noexcept");
                        headerText += IndentedMLine($"{{");
                        for (int i = 0; i < aclass.Properties.Count; ++i)
                        {
                            Indented(() =>
                            {
                                headerText += IndentedMLine($"if constexpr (N == {count++})");
                                headerText += IndentedMLine($"{{");
                                Indented(() =>
                                {
                                    headerText += IndentedMLine($"return {aclass.Properties[i].Variable.Name}();");
                                });
                                headerText += IndentedMLine($"}}");
                            });
                        }
                        for (int i = 0; i < aclass.Constructors.Count; ++i)
                        {
                            Indented(() =>
                            {
                                headerText += IndentedMLine($"if constexpr (N == {count++})");
                                headerText += IndentedMLine($"{{");
                                Indented(() =>
                                {
                                    headerText += IndentedMLine($"return {aclass.Constructors[i].Name}__{i}();");
                                });
                                headerText += IndentedMLine($"}}");
                            });
                        }
                        for (int i = 0; i < aclass.Functions.Count; ++i)
                        {
                            Indented(() =>
                            {
                                headerText += IndentedMLine($"if constexpr (N == {count++})");
                                headerText += IndentedMLine($"{{");
                                Indented(() =>
                                {
                                    headerText += IndentedMLine($"return {aclass.Functions[i].Name}__{i}();");
                                });
                                headerText += IndentedMLine($"}}");
                            });
                        }
                        if (count > 0)
                        {
                            Indented(() =>
                            {
                                headerText += IndentedMLine($"else");
                            });
                        }
                        Indented(() =>
                        {
                            headerText += IndentedMLine($"{{");
                            Indented(() =>
                            {
                                headerText += IndentedMLine($"static_assert(N < {count}, \"Invalid reflexpr index\");");
                            });
                            headerText += IndentedMLine($"}}");
                        });
                        headerText += IndentedMLine($"}}");
                        headerText += IndentedMLine($"");
                        headerText += IndentedMLine($"static constexpr size_t tuple_size = {count};");
                    });
                    headerText += IndentedMLine($"}};");
                });
                headerText += Indented_Line($"}};");
                headerText += Indented_Line($"");
                headerText += IndentedMLine($"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber}__script_table ");
                Indented(() =>
                {
                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                        var function = aclass.Functions[i];
                        if (function.Flags.HasFlag(SFunction.FFlags.Virtual) == false)
                        {
                            continue;
                        }

                        var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                        var @params = string.Join(", ", function.Parameters.Select(p => $"{p.Variable.TypeName.FullName} {p.Variable.Name}"));
                        var suffix = function.Flags.HasFlag(SFunction.FFlags.Const) ? " const" : string.Empty;
                        suffix += function.Flags.HasFlag(SFunction.FFlags.Pure) ? " = 0" : string.Empty;
                        headerText += IndentedMLine($"virtual {function.ReturnType.FullName} {function.Name}_Implementation({@params}){suffix};");
                    }
                });
                headerText += Indented_Line($"");
                headerText += IndentedMLine($"#define GENERATED_BODY__IMPL__{m_FileId}__{lineNumber} ");
                Indented(() =>
                {
                    headerText += IndentedMLine($"GENERATED_BODY__DEFAULT_BODY({aclass.Class.Name}) ");
                    headerText += IndentedMLine($"GENERATED_BODY__IMPL__{m_FileId}__{lineNumber}__reflexpr_class");
                    headerText += IndentedMLine($"GENERATED_BODY__IMPL__{m_FileId}__{lineNumber}__script_table");
                    foreach (var friend in friends)
                    {
                        headerText += IndentedMLine(friend);
                    }
                    headerText += IndentedMLine($"GENERATED_BODY__DECLARE_GATHER_PROPERTIES()");
                    headerText += IndentedMLine($"GENERATED_BODY__DECLARE_TRANSFER()");
                    headerText += IndentedMLine($"GENERATED_BODY__DECLARE_MANAGED_TYPE()");
                });
                headerText += Indented_Line($"private:\n");
            }

            if (syntax is SAEnum aenum)
            {
                string cppnamespace = string.Join("::", aenum.Namespaces.Select(p => p.Name));
                string fullname = "::" + string.Join("::", aenum.Namespaces.Select(p => p.Name).Append(aenum.Name));
                string functionName = $"{cppnamespace.Replace("::", "__")}__{aenum.Name}__ToString";

                headerText += Indented_Line($"namespace {cppnamespace}");
                headerText += Indented_Line($"{{");
                Indented(() =>
                {
                    headerText += Indented_Line($"enum class {aenum.Name};");
                });
                headerText += Indented_Line($"}}");
                headerText += Indented_Line($"");
                headerText += Indented_Line($"PLATFORM_SHARED_EXPORT ::Ayla::String {functionName}({fullname} value);");
                headerText += Indented_Line($"");

                headerText += Indented_Line($"template<class TChar> requires (::std::same_as<TChar, char> || ::std::same_as<TChar, wchar_t>)");
                headerText += Indented_Line($"struct std::formatter<{fullname}, TChar> : public ::std::formatter<::Ayla::String, TChar>");
                headerText += Indented_Line($"{{");
                Indented(() =>
                {
                    headerText += Indented_Line($"template<class TFormatContext>");
                    headerText += Indented_Line($"auto format({fullname} value, TFormatContext& ctx) const");
                    headerText += Indented_Line($"{{");
                    Indented(() =>
                    {
                        headerText += Indented_Line($"return ::std::formatter<::Ayla::String, TChar>::format({functionName}(value), ctx);");
                    });
                    headerText += Indented_Line($"}}");
                });
                headerText += Indented_Line($"}};");
            }
        }

        return headerText;
        
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

        string Indented_Line(string text)
        {
            return new string(' ', indent * 2) + text + "\n";
        }

        string IndentedMLine(string text)
        {
            return Indented_Line(text + '\\');
        }
    }
}
