using AylaEngine.RHT.Syntaxes;
using AylaEngine.RHT.Types;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateCSharp(TypeNames typeNames)
    {
        string sourceCode = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

using System.Runtime.InteropServices;


""";

        int indent = 0;
        var moduleName = SourceCode.ModuleName;

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is SAClass aclass)
            {
                var @class = typeNames.FindClass(aclass.Class);
                if (@class.Namespace.Names.Length > 0)
                {
                    sourceCode += IndentedLine($"namespace {string.Join(".", @class.Namespace.Names)}");
                    sourceCode += IndentedLine($"{{");
                    ++indent;
                }

                var @base = aclass.Class.Base;
                string inherit = string.Empty;
                if (@base != null)
                {
                    var baseClass = typeNames.FindType(@base, aclass.Class);
                    inherit = $" : {baseClass.CSharpName}";
                }

                sourceCode += IndentedLine($"public class {@class.Name}__Injected{inherit}");
                sourceCode += IndentedLine($"{{");
                Indented(() =>
                {
                    sourceCode += IndentedLine($"protected {@class.Name}__Injected(global::Ayla.ObjectReferenceLocker locker) : base(locker)");
                    sourceCode += IndentedLine($"{{");
                    sourceCode += IndentedLine($"}}");
                    sourceCode += IndentedLine($"");

                    for (int i = 0; i < aclass.Constructors.Count; ++i)
                    {
                        var constructor = aclass.Constructors[i];
                        var returnType = (SharedPtrTypeName)Activator.CreateInstance(typeof(SharedPtrTypeName), @class)!;
                        var parameterTypes = constructor.Parameters.Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class)).ToArray();

                        var injectParamsDeclare = string.Join(", ", parameterTypes.Select((t, i) => $"{t.CSharpBindingName} {constructor.Parameters[i].Variable.Name}"));
                        string nativeFunctionName = $"{string.Join("__", @class.Namespace.Names)}__{@class.Name}__{constructor.Name}__{i}__Injected";
                        sourceCode += IndentedLine($"[DllImport(\"{moduleName}\", EntryPoint = \"{nativeFunctionName}\")]");
                        sourceCode += IndentedLine($"private static extern global::Ayla.ObjectReferenceLocker ctor_{constructor.Name}__Injected({injectParamsDeclare});");

                        var internalParamsDeclare = string.Join(", ", parameterTypes.Select((t, i) => $"{t.CSharpName} {constructor.Parameters[i].Variable.Name}"));
                        var returnStmt = returnType.CSharpName;
                        // TODO: support parameters
                        sourceCode += IndentedLine($"public unsafe {constructor.Name}__Injected({internalParamsDeclare}) : this(ctor_{constructor.Name}__Injected())");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");
                    }

                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                        var function = aclass.Functions[i];
                        var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                        var parameterTypes = function.Parameters.Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class)).ToArray();
                        bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);
                        bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);

                        var injectParamsDeclare = string.Join(", ", parameterTypes.Select((t, i) => $"{t.CSharpBindingName} {function.Parameters[i].Variable.Name}"));
                        string nativeFunctionName = $"{string.Join("__", @class.Namespace.Names)}__{@class.Name}__{function.Name}__{i}__Injected";
                        if (isStatic == false)
                        {
                            injectParamsDeclare = "nint self" + (injectParamsDeclare.Length > 0 ? ", " : string.Empty) + injectParamsDeclare;
                        }
                        sourceCode += IndentedLine($"[DllImport(\"{moduleName}\", EntryPoint = \"{nativeFunctionName}\")]");
                        sourceCode += IndentedLine($"private static extern {returnType.CSharpBindingName} {function.Name}__Injected({injectParamsDeclare});");

                        var internalParamsDeclare = string.Join(", ", parameterTypes.Select((t, i) => $"{t.CSharpName} {function.Parameters[i].Variable.Name}"));
                        var returnStmt = returnType is SharedPtrTypeName ? $"{returnType.CSharpName}?" : returnType.CSharpName;
                        sourceCode += IndentedLine($"public unsafe{(isVirtual ? " virtual" : string.Empty)}{(isStatic ? " static" : string.Empty)} {returnStmt} {function.Name}({internalParamsDeclare})");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            GenerateFunctionBody(function, string.Empty, parameterTypes, function.Parameters, returnType, isStatic, false);
                        });
                        sourceCode += IndentedLine($"}}");
                        if (isVirtual)
                        {
                            sourceCode += IndentedLine($"private static unsafe {returnType.CSharpBindingName} {function.Name}__Invoke(global::Ayla.ObjectReferenceWrapper self_)");
                            sourceCode += IndentedLine($"{{");
                            Indented(() =>
                            {
                                sourceCode += IndentedLine($"var self = self_.As<{aclass.Class.Name}>()!;");
                                string bodyStatement = $"self.{function.Name}()";
                                if (returnType == TypeName.Void)
                                {
                                    sourceCode += IndentedLine(bodyStatement + ';');
                                }
                                else if (returnType is SharedPtrTypeName)
                                {
                                    sourceCode += IndentedLine($"return global::Ayla.Marshaller.AsBinding({bodyStatement});");
                                }
                                else
                                {
                                    sourceCode += IndentedLine($"return {bodyStatement};");
                                }
                            });
                            sourceCode += IndentedLine($"}}");
                        }
                        sourceCode += IndentedLine($"");
                    }

                    void GenerateFunctionBody(SMember member, string prefix, TypeName[] parameterTypes, SParameter[] parameters, TypeName returnType, bool isStatic, bool returnAsBinding)
                    {
                        List<string> allocateStatements = [];
                        List<string> fixedStatements = [];
                        List<string> arguments = [];
                        List<string> releaseStatements = [];

                        for (int j = 0; j < parameterTypes.Length; ++j)
                        {
                            var paramType = parameterTypes[j];
                            var parameter = parameters[j].Variable;
                            if (paramType == TypeName.String)
                            {
                                fixedStatements.Add($"fixed (char* {parameter.Name}_ptr = {parameter.Name})");
                                arguments.Add($"(nint){parameter.Name}_ptr");
                            }
                            else if (paramType is ArrayTypeName arrayType)
                            {
                                if (arrayType.ElementType == TypeName.String)
                                {
                                    allocateStatements.Add($"nint {parameter.Name}_ptr = Marshaller.StringArrayToNative({parameter.Name});");
                                    releaseStatements.Add($"Marshaller.ReleaseStringArray({parameter.Name}_ptr, {parameter.Name}.Length);");
                                    arguments.Add(parameter.Name + "_ptr");
                                }
                                else if (arrayType.ElementType is SharedPtrTypeName)
                                {
                                    allocateStatements.Add($"nint {parameter.Name}_ptr = Marshaller.ObjectArrayToNative({parameter.Name});");
                                    releaseStatements.Add($"Marshaller.ReleaseObjectArray({parameter.Name}_ptr);");
                                    arguments.Add(parameter.Name + "_ptr");
                                }
                                else
                                {
                                    fixedStatements.Add($"fixed ({arrayType.ElementType.CSharpBindingName}* {parameter.Name}_ptr = {parameter.Name})");
                                    arguments.Add("(nint)" + parameter.Name + "_ptr");
                                }
                            }
                            else
                            {
                                arguments.Add(parameter.Name);
                            }
                        }

                        if (allocateStatements.Count > 0)
                        {
                            foreach (var stmt in allocateStatements)
                            {
                                sourceCode += IndentedLine(stmt);
                            }
                            sourceCode += IndentedLine($"try");
                            sourceCode += IndentedLine($"{{");
                            ++indent;
                        }
                        try
                        {
                            if (fixedStatements.Count > 0)
                            {
                                foreach (var stmt in fixedStatements)
                                {
                                    sourceCode += IndentedLine(stmt);
                                }

                                sourceCode += IndentedLine($"{{");
                                ++indent;
                            }

                            try
                            {
                                if (isStatic == false)
                                {
                                    arguments.Insert(0, "this.NativePointer");
                                }

                                string bodyStmt = $"{prefix}{member.Name}__Injected({string.Join(", ", arguments)})";

                                if (returnType == TypeName.Void)
                                {
                                    sourceCode += IndentedLine(bodyStmt + ";");
                                }
                                else if (returnType is SharedPtrTypeName && returnAsBinding == false)
                                {
                                    sourceCode += IndentedLine($"return {bodyStmt}.As<{returnType.CSharpName}>();");
                                }
                                else
                                {
                                    sourceCode += IndentedLine($"return {bodyStmt};");
                                }
                            }
                            finally
                            {
                                if (fixedStatements.Count > 0)
                                {
                                    --indent;
                                    sourceCode += IndentedLine($"}}");
                                }
                            }
                        }
                        finally
                        {
                            if (allocateStatements.Count > 0)
                            {
                                --indent;
                                sourceCode += IndentedLine($"}}");
                                sourceCode += IndentedLine($"finally");
                                sourceCode += IndentedLine($"{{");
                                Indented(() =>
                                {
                                    foreach (var stmt in releaseStatements)
                                    {
                                        sourceCode += IndentedLine(stmt);
                                    }
                                });
                                sourceCode += IndentedLine($"}}");
                            }
                        }
                    }
                });
                sourceCode += IndentedLine($"}}");
                sourceCode += IndentedLine($"");

                sourceCode += IndentedLine($"public partial class {@class.Name} : {@class.Name}__Injected");
                sourceCode += IndentedLine($"{{");

                Indented(() =>
                {
sourceCode += "#pragma warning disable CS8618\n";

                    sourceCode += IndentedLine($"protected {@class.Name}(global::Ayla.ObjectReferenceLocker locker) : base(locker)");
                    sourceCode += IndentedLine($"{{");
                    sourceCode += IndentedLine($"}}");

sourceCode += "#pragma warning restore CS8618\n";
                });

                sourceCode += IndentedLine($"}}");

                if (@class.Namespace.Names.Length > 0)
                {
                    --indent;
                    sourceCode += IndentedLine($"}}");
                }
            }
        }

        return sourceCode;

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

        string IndentedLine(string text)
        {
            return new string(' ', indent * 4) + text + "\n";
        }
    }
}
