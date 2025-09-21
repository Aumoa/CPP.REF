using AylaEngine.RHT.CodeGen;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateCSharp(ModuleProject project, TargetInfo buildTarget, TypeNames typeNames)
    {
        string sourceCode = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#nullable disable

using System.Runtime.InteropServices;


""";

        int indent = 0;
        var moduleName = SourceCode.ModuleName;

        foreach (var syntax in m_Syntaxes)
        {
            if (syntax is SAEnum aenum)
            {
                var @enum = typeNames.FindEnum(aenum);
                if (@enum.Namespace.Names.Length > 0)
                {
                    sourceCode += IndentedLine($"namespace {string.Join(".", @enum.Namespace.Names)}");
                    sourceCode += IndentedLine($"{{");
                    ++indent;
                }
                sourceCode += IndentedLine($"public enum {@enum.Name}");
                sourceCode += IndentedLine($"{{");
                Indented(() =>
                {
                    for (int i = 0; i < aenum.Defines.Length; ++i)
                    {
                        var value = aenum.Defines[i];
                        string comma = i < aenum.Defines.Length - 1 ? "," : string.Empty;
                        if (value.Value == null)
                        {
                            sourceCode += IndentedLine($"{value.Name}{comma}");
                        }
                        else
                        {
                            sourceCode += IndentedLine($"{value.Name} = {value.Value}{comma}");
                        }
                    }
                });
                sourceCode += IndentedLine($"}}");
                sourceCode += IndentedLine($"");
                if (@enum.Namespace.Names.Length > 0)
                {
                    --indent;
                    sourceCode += IndentedLine($"}}");
                }
            }

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

                sourceCode += IndentedLine($"public abstract class {@class.Name}__Injected{inherit}");
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
                        var access = constructor.Access.ToString().ToLower();
                        var returnType = (SharedPtrTypeName)Activator.CreateInstance(typeof(SharedPtrTypeName), @class)!;
                        var parameters = new ParameterCollection();
                        foreach (var param in constructor.Parameters)
                        {
                            var paramType = typeNames.FindType(param.Variable.TypeName, aclass.Class);
                            parameters.Add(paramType, param.Variable.Name);
                        }

                        var injectParamsDeclare = ParametersGenerator.GenerateCSharpBindings(parameters);
                        string nativeFunctionName = $"{string.Join("__", @class.Namespace.Names)}__{@class.Name}__{constructor.Name}__{i}__Injected";
                        sourceCode += IndentedLine($"[DllImport(\"{moduleName}\", EntryPoint = \"{nativeFunctionName}\")]");
                        sourceCode += IndentedLine($"private static extern global::Ayla.ObjectReferenceLocker ctor_{constructor.Name}__Injected({injectParamsDeclare});");

                        var csharpParamsDeclare = ParametersGenerator.GenerateCSharp(parameters);
                        sourceCode += IndentedLine($"private static unsafe global::Ayla.ObjectReferenceLocker ctor_{constructor.Name}__CallInjected({csharpParamsDeclare})");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            var codegen = new FunctionBodyGenerator(parameters, $"ctor_{constructor.Name}__Injected", TypeName.Object);
                            codegen.GenerateCSharpCSharpToNative(ref sourceCode, ref indent, IndentedLine);
                        });
                        sourceCode += IndentedLine($"}}");

                        var returnStmt = returnType.CSharpName;
                        var callArguments = FunctionBodyGenerator.GeneratePassArguments(parameters);
                        sourceCode += IndentedLine($"protected unsafe {constructor.Name}__Injected({csharpParamsDeclare}) : this(ctor_{constructor.Name}__CallInjected({callArguments}))");
                        sourceCode += IndentedLine($"{{");
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");
                    }

                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                        var function = aclass.Functions[i];
                        var access = function.Access.ToString().ToLower();
                        var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                        var parameterTypes = function.Parameters.Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class)).ToArray();
                        bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);
                        bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);
                        var parameters = new ParameterCollection();

                        for (int j = 0; j < parameterTypes.Length; ++j)
                        {
                            var paramType = parameterTypes[j];
                            var parameter = function.Parameters[j].Variable;
                            parameters.Add(paramType, parameter.Name);
                        }

                        string nativeFunctionName = $"{string.Join("__", @class.Namespace.Names)}__{@class.Name}__{function.Name}__{i}__Injected";
                        string injectParamsDeclare;
                        if (isStatic)
                        {
                            injectParamsDeclare = ParametersGenerator.GenerateCSharpBindings(parameters);
                        }
                        else
                        {
                            injectParamsDeclare = ParametersGenerator.GenerateCSharpBindings(parameters.AddFirstTemp(TypeName.IntPtr, "self"));
                        }

                        sourceCode += IndentedLine($"[DllImport(\"{moduleName}\", EntryPoint = \"{nativeFunctionName}\")]");
                        sourceCode += IndentedLine($"private static extern {returnType.CSharpBindingName} {function.Name}__Injected({injectParamsDeclare});");

                        var internalParamsDeclare = string.Join(", ", parameterTypes.Select((t, i) => $"{t.CSharpName} {function.Parameters[i].Variable.Name}"));
                        var returnStmt = returnType is SharedPtrTypeName ? $"{returnType.CSharpName}" : returnType.CSharpName;
                        sourceCode += IndentedLine($"{access} unsafe{(isVirtual ? " virtual" : string.Empty)}{(isStatic ? " static" : string.Empty)} {returnStmt} {function.Name}({internalParamsDeclare})");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            var rule = project.GetRule(buildTarget);
                            if (rule.Type == ModuleType.Application)
                            {
                                sourceCode += IndentedLine($"throw new global::System.AccessViolationException(\"Assemblies of the Application type cannot directly invoke native functions.\");");
                                return;
                            }

                            FunctionBodyGenerator bodyGen;
                            string callable = $"{function.Name}__Injected";
                            if (isStatic == false)
                            {
                                bodyGen = new FunctionBodyGenerator(parameters.AddFirstTemp(TypeName.IntPtr, "this.NativePointer"), callable, returnType);
                            }
                            else
                            {
                                bodyGen = new FunctionBodyGenerator(parameters, callable, returnType);
                            }

                            bodyGen.GenerateCSharpCSharpToNative(ref sourceCode, ref indent, IndentedLine);
                        });
                        sourceCode += IndentedLine($"}}");
                        if (isVirtual)
                        {
                            var invokeParamsDeclare = ParametersGenerator.GenerateCSharpBindings(parameters.AddFirstTemp(TypeName.Object, "self_"));
                            sourceCode += IndentedLine($"private static unsafe {returnType.CSharpBindingName} {function.Name}__Invoke({invokeParamsDeclare})");
                            sourceCode += IndentedLine($"{{");
                            Indented(() =>
                            {
                                sourceCode += IndentedLine($"var self = self_.AsManaged<{aclass.Class.Name}>()!;");
                                string callable = $"self.{function.Name}";
                                var codeGen = new FunctionBodyGenerator(parameters, callable, returnType);
                                codeGen.GenerateCSharpNativeToCSharp(ref sourceCode, ref indent, IndentedLine);
                            });
                            sourceCode += IndentedLine($"}}");
                        }
                        sourceCode += IndentedLine($"");
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
                    Indented(() =>
                    {
                        sourceCode += IndentedLine($"OnConstructed();");
                    });
                    sourceCode += IndentedLine($"}}");
                    sourceCode += IndentedLine($"partial void OnConstructed();");

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
