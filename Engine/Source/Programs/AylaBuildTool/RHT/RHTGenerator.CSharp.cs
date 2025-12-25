using AylaEngine.RHT.CodeGen;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public string GenerateCSharp(ModuleProject project, TargetInfo buildTarget, TypeNames typeNames)
    {
        var generator = new CSharpCodeGenerator(this, project, buildTarget, typeNames);
        return generator.Generate();
    }

    [Obsolete("Use GenerateCSharp instead. This method is kept for reference.")]
    public string GenerateCSharp_Legacy(ModuleProject project, TargetInfo buildTarget, TypeNames typeNames)
    {
        const string kDllImport = "global::System.Runtime.InteropServices.DllImport";

        string sourceCode = $"""
// Copyright 2020-2025 AylaEngine. All Rights Reserved.
// This file is auto-generated. Do not edit it manually.

#nullable disable


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
                var @namespace = string.Join(".", @class.Namespace.Names);
                if (@class.Namespace.Names.Length > 0)
                {
                    sourceCode += IndentedLine($"namespace {@namespace}");
                    sourceCode += IndentedLine($"{{");
                    ++indent;
                }

                string injectClassName = $"{@class.Name}__Injected";
                string injectFullName = $"global::{@namespace}.{injectClassName}";

                string invocableClassName = $"{@class.Name}__Invocable";
                string invocableFullName = $"global::{@namespace}.{invocableClassName}";

                string classFullName = $"global::{@namespace}.{@class.Name}";

                sourceCode += IndentedLine($"file static class {injectClassName}");
                sourceCode += IndentedLine($"{{");
                Indented(() =>
                {
                    sourceCode += IndentedLine($"[{kDllImport}(\"{moduleName}\", EntryPoint = \"{@class.CppName[2..].Replace("::", "__")}__GetManagedType\")]");
                    sourceCode += IndentedLine($"public static extern global::Ayla.ManagedTypeWrapper GetManagedType();");

                    for (int i = 0; i < aclass.Constructors.Count; ++i)
                    {
                        var constructor = aclass.Constructors[i];
                        var parameters = new ParameterCollection();
                        foreach (var param in constructor.Parameters)
                        {
                            var paramType = typeNames.FindType(param.Variable.TypeName, aclass.Class);
                            parameters.Add(paramType, param.Variable.Name);
                        }

                        var injectParamsDeclare = ParametersGenerator.GenerateCSharpBindings(parameters.AddFirstTemp(TypeName.IntPtr, "__gchandle_ptr"));
                        string nativeFunctionName = $"{string.Join("__", @class.Namespace.Names)}__{@class.Name}__{constructor.Name}__{i}__Injected";
                        sourceCode += IndentedLine($"[{kDllImport}(\"{moduleName}\", EntryPoint = \"{nativeFunctionName}\")]");
                        sourceCode += IndentedLine($"public static extern nint ctor_{constructor.Name}({injectParamsDeclare});");
                    }

                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                        var function = aclass.Functions[i];
                        var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                        var parameterTypes = function.Parameters.Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class)).ToArray();
                        bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);
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

                        sourceCode += IndentedLine($"[{kDllImport}(\"{moduleName}\", EntryPoint = \"{nativeFunctionName}\")]");
                        sourceCode += IndentedLine($"public static extern {returnType.CSharpBindingName} {function.Name}({injectParamsDeclare});");
                    }
                });
                sourceCode += IndentedLine($"}}");
                sourceCode += IndentedLine($"");

                var @base = aclass.Class.Base;
                string inherit = string.Empty;
                if (@base != null)
                {
                    var baseClass = typeNames.FindType(@base, aclass.Class);
                    inherit = $" : {baseClass.CSharpName}";
                }

                sourceCode += IndentedLine($"public abstract class {invocableClassName}{inherit}");
                sourceCode += IndentedLine($"{{");
                Indented(() =>
                {
                    sourceCode += IndentedLine($"protected {invocableClassName}(global::System.Func<object, nint> locker) : base(locker)");
                    sourceCode += IndentedLine($"{{");
                    sourceCode += IndentedLine($"}}");
                    sourceCode += IndentedLine($"");
                    sourceCode += IndentedLine($"private static readonly global::Ayla.GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof({classFullName});");
                    sourceCode += IndentedLine($"private static nint GetScriptType__Invoke() => global::System.Runtime.InteropServices.Marshal.GetFunctionPointerForDelegate(s_GetScriptType__Delegate);");

                    for (int i = 0; i < aclass.Constructors.Count; ++i)
                    {
                        var constructor = aclass.Constructors[i];
                        var returnType = (SharedPtrTypeName)Activator.CreateInstance(typeof(SharedPtrTypeName), @class)!;
                        var parameters = new ParameterCollection();
                        foreach (var param in constructor.Parameters)
                        {
                            var paramType = typeNames.FindType(param.Variable.TypeName, aclass.Class);
                            parameters.Add(paramType, param.Variable.Name);
                        }

                        var csharpParamsDeclare = ParametersGenerator.GenerateCSharp(parameters);
                        var returnStmt = returnType.CSharpName;
                        var callArguments = FunctionBodyGenerator.GeneratePassArguments(parameters.AddFirstTemp(TypeName.IntPtr, "__gchandle_ptr"));
                        sourceCode += IndentedLine($"protected unsafe {invocableClassName}({csharpParamsDeclare}) : this(@this =>");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            var codegen = new FunctionBodyGenerator(parameters.AddFirstTemp(PlaceholderName.Value, "(nint)global::System.Runtime.InteropServices.GCHandle.Alloc(@this, global::System.Runtime.InteropServices.GCHandleType.Weak)"), $"{injectFullName}.ctor_{constructor.Name}", TypeName.Object);
                            codegen.GenerateCSharpCSharpToNative(ref sourceCode, ref indent, IndentedLine);
                        });
                        sourceCode += IndentedLine($"}})");
                        sourceCode += IndentedLine($"{{");
                        sourceCode += IndentedLine($"}}");
                    }

                    sourceCode += IndentedLine($"");

                    GenerateFunctionBodies(true);
                });
                sourceCode += IndentedLine($"}}");
                sourceCode += IndentedLine($"");

                sourceCode += IndentedLine($"public partial class {@class.Name} : {invocableFullName}, global::Ayla.IStaticObject");
                sourceCode += IndentedLine($"{{");

                Indented(() =>
                {
                    sourceCode += "#pragma warning disable CS8618\n";

                    sourceCode += IndentedLine($"protected {@class.Name}(global::System.Func<object, nint> locker) : base(locker)");
                    sourceCode += IndentedLine($"{{");
                    Indented(() =>
                    {
                        sourceCode += IndentedLine($"OnConstructed(locker);");
                    });
                    sourceCode += IndentedLine($"}}");
                    sourceCode += "#pragma warning restore CS8618\n";
                    sourceCode += IndentedLine($"partial void OnConstructed(global::System.Func<object, nint> locker);");
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

                        var csharpParamsDeclare = ParametersGenerator.GenerateCSharp(parameters);

                        var callArguments = FunctionBodyGenerator.GeneratePassArguments(parameters);
                        sourceCode += IndentedLine($"{access} {constructor.Name}({csharpParamsDeclare}) : base({callArguments})");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"OnConstructed({callArguments});");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"partial void OnConstructed({csharpParamsDeclare});");
                    }

                    sourceCode += IndentedLine($"");
                    sourceCode += IndentedLine($"public override global::Ayla.ManagedTypeWrapper GetClass() => StaticClass();");
                    sourceCode += IndentedLine($"");
                    sourceCode += IndentedLine($"public static new global::Ayla.ManagedTypeWrapper StaticClass() => {injectFullName}.GetManagedType();");

                    GenerateFunctionBodies(false);
                });

                sourceCode += IndentedLine($"}}");

                if (@class.Namespace.Names.Length > 0)
                {
                    --indent;
                    sourceCode += IndentedLine($"}}");
                }

                void GenerateFunctionBodies(bool acceptVirtual)
                {
                    for (int i = 0; i < aclass.Functions.Count; ++i)
                    {
                        var function = aclass.Functions[i];
                        bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);

                        if (isVirtual != acceptVirtual)
                        {
                            continue;
                        }

                        var access = function.Access.ToString().ToLower();
                        var returnType = typeNames.FindType(function.ReturnType, aclass.Class);
                        var parameterTypes = function.Parameters.Select(p => typeNames.FindType(p.Variable.TypeName, aclass.Class)).ToArray();
                        bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);
                        var parameters = new ParameterCollection();

                        for (int j = 0; j < parameterTypes.Length; ++j)
                        {
                            var paramType = parameterTypes[j];
                            var parameter = function.Parameters[j].Variable;
                            parameters.Add(paramType, parameter.Name);
                        }

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
                            string callable = $"{injectFullName}.{function.Name}";
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
                }
            }
            
            if (syntax is SDefineLogCategory defineLogCategory)
            {
                sourceCode += IndentedLine($"namespace {string.Join(".", defineLogCategory.Namespaces.Select(n => n.Name))}");
                sourceCode += IndentedLine($"{{");
                Indented(() =>
                {
                    sourceCode += IndentedLine($"public static class {defineLogCategory.Name}");
                    sourceCode += IndentedLine($"{{");
                    Indented(() =>
                    {
                        sourceCode += IndentedLine($"public const string kCategory = \"{defineLogCategory.Name}\";");
                        sourceCode += IndentedLine($"");
                        
                        // Log methods with LogVerbosity parameter
                        sourceCode += IndentedLine($"public static void Log(global::Ayla.LogVerbosity level, string message)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.Log(kCategory, level, message);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        sourceCode += IndentedLine($"public static void Log(global::Ayla.LogVerbosity level, string format, params global::System.ReadOnlySpan<object> args)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.Log(kCategory, level, format, args);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        // Verbose level methods
                        sourceCode += IndentedLine($"public static void Verbose(string message)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogVerbose(kCategory, message);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        sourceCode += IndentedLine($"public static void Verbose(string format, params System.ReadOnlySpan<object> args)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogVerbose(kCategory, format, args);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        // Info level methods
                        sourceCode += IndentedLine($"public static void Info(string message)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogInfo(kCategory, message);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        sourceCode += IndentedLine($"public static void Info(string format, params System.ReadOnlySpan<object> args)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogInfo(kCategory, format, args);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        // Warning level methods
                        sourceCode += IndentedLine($"public static void Warning(string message)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogWarning(kCategory, message);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        sourceCode += IndentedLine($"public static void Warning(string format, params System.ReadOnlySpan<object> args)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogWarning(kCategory, format, args);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        // Error level methods
                        sourceCode += IndentedLine($"public static void Error(string message)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogError(kCategory, message);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        sourceCode += IndentedLine($"public static void Error(string format, params System.ReadOnlySpan<object> args)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogError(kCategory, format, args);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        // Critical level methods
                        sourceCode += IndentedLine($"public static void Critical(string message)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogCritical(kCategory, message);");
                        });
                        sourceCode += IndentedLine($"}}");
                        sourceCode += IndentedLine($"");

                        sourceCode += IndentedLine($"public static void Critical(string format, params System.ReadOnlySpan<object> args)");
                        sourceCode += IndentedLine($"{{");
                        Indented(() =>
                        {
                            sourceCode += IndentedLine($"global::Ayla.Debug.LogCritical(kCategory, format, args);");
                        });
                        sourceCode += IndentedLine($"}}");
                    });
                    sourceCode += IndentedLine($"}}");
                });
                sourceCode += IndentedLine($"}}");
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
