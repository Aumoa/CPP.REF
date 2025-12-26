// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C# class reflection code.
/// </summary>
internal class CSharpClassGenerator
{
    private const string kDllImport = "global::System.Runtime.InteropServices.DllImport";
    
    private readonly CSharpCodeGenerator m_Parent;
    private readonly SAClass m_Class;
    private string m_SourceCode = string.Empty;

    public CSharpClassGenerator(CSharpCodeGenerator parent, SAClass aclass)
    {
        m_Parent = parent;
        m_Class = aclass;
    }

    public string Generate()
    {
        var @class = m_Parent.TypeNames.FindClass(m_Class.Class);
        var @namespace = string.Join(".", @class.Namespace.Names);

        if (@class.Namespace.Names.Length > 0)
        {
            m_SourceCode += m_Parent.IndentedLine($"namespace {@namespace}");
            m_SourceCode += m_Parent.IndentedLine("{");
            m_Parent.Indent();
        }

        string injectClassName = $"{@class.Name}__Injected";
        string injectFullName = $"global::{@namespace}.{injectClassName}";
        string invocableClassName = $"{@class.Name}__Invocable";
        string invocableFullName = $"global::{@namespace}.{invocableClassName}";
        string classFullName = $"global::{@namespace}.{@class.Name}";

        GenerateInjectedClass(injectClassName);
        GenerateInvocableClass(@class, injectFullName, invocableClassName, classFullName);
        GenerateMainClass(@class, invocableFullName, injectFullName);

        if (@class.Namespace.Names.Length > 0)
        {
            m_Parent.Dedent();
            m_SourceCode += m_Parent.IndentedLine("}");
        }

        return m_SourceCode;
    }

    private void GenerateInjectedClass(string injectClassName)
    {
        m_SourceCode += m_Parent.IndentedLine($"file static class {injectClassName}");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            GenerateInjectedMethods();
        });
        m_SourceCode += m_Parent.IndentedLine("}");
        m_SourceCode += m_Parent.IndentedLine("");
    }

    private void GenerateInjectedMethods()
    {
        var @class = m_Parent.TypeNames.FindClass(m_Class.Class);
        
        // GetManagedType method
        m_SourceCode += m_Parent.IndentedLine($"[{kDllImport}(\"{m_Parent.ModuleName}\", EntryPoint = \"{@class.CppName[2..].Replace("::", "__")}__GetManagedType\")]");
        m_SourceCode += m_Parent.IndentedLine($"public static extern global::Ayla.ManagedTypeWrapper GetManagedType();");

        // Constructor imports
        for (int i = 0; i < m_Class.Constructors.Count; ++i)
        {
            GenerateInjectedConstructor(i, @class);
        }

        // Function imports
        for (int i = 0; i < m_Class.Functions.Count; ++i)
        {
            GenerateInjectedFunction(i, @class);
        }
    }

    private void GenerateInjectedConstructor(int index, ClassName @class)
    {
        var constructor = m_Class.Constructors[index];
        var parameters = CollectParameters(constructor.Parameters);
        var injectParamsDeclare = ParametersGenerator.GenerateCSharpBindings(parameters.AddFirstTemp(TypeName.IntPtr, "__gchandle_ptr"));
        string nativeFunctionName = $"{string.Join("__", @class.Namespace.Names)}__{@class.Name}__{constructor.Name}__{index}__Injected";
        
        m_SourceCode += m_Parent.IndentedLine($"[{kDllImport}(\"{m_Parent.ModuleName}\", EntryPoint = \"{nativeFunctionName}\")]");
        m_SourceCode += m_Parent.IndentedLine($"public static extern nint ctor_{constructor.Name}({injectParamsDeclare});");
    }

    private void GenerateInjectedFunction(int index, ClassName @class)
    {
        var function = m_Class.Functions[index];
        var returnType = m_Parent.TypeNames.FindType(function.ReturnType, m_Class.Class);
        var parameters = CollectParameters(function.Parameters);
        bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);

        string nativeFunctionName = $"{string.Join("__", @class.Namespace.Names)}__{@class.Name}__{function.Name}__{index}__Injected";
        string injectParamsDeclare = isStatic
            ? ParametersGenerator.GenerateCSharpBindings(parameters)
            : ParametersGenerator.GenerateCSharpBindings(parameters.AddFirstTemp(TypeName.IntPtr, "self"));

        m_SourceCode += m_Parent.IndentedLine($"[{kDllImport}(\"{m_Parent.ModuleName}\", EntryPoint = \"{nativeFunctionName}\")]");
        m_SourceCode += m_Parent.IndentedLine($"public static extern {returnType.CSharpBindingName} {function.Name}({injectParamsDeclare});");
    }

    private void GenerateInvocableClass(ClassName @class, string injectFullName, string invocableClassName, string classFullName)
    {
        var @base = m_Class.Class.Base;
        string inherit = string.Empty;
        if (@base != null)
        {
            var baseClass = m_Parent.TypeNames.FindType(@base, m_Class.Class);
            inherit = $" : {baseClass.CSharpName}";
        }

        m_SourceCode += m_Parent.IndentedLine($"public abstract class {invocableClassName}{inherit}");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            GenerateInvocableMembers(@class, injectFullName, classFullName);
        });
        m_SourceCode += m_Parent.IndentedLine("}");
        m_SourceCode += m_Parent.IndentedLine("");
    }

    private void GenerateInvocableMembers(ClassName @class, string injectFullName, string classFullName)
    {
        // Base constructor
        m_SourceCode += m_Parent.IndentedLine($"protected {m_Class.Class.Name}__Invocable(global::System.Func<object, nint> locker) : base(locker)");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_SourceCode += m_Parent.IndentedLine("}");
        m_SourceCode += m_Parent.IndentedLine("");
        
        // Static type getter
        m_SourceCode += m_Parent.IndentedLine($"private static readonly global::Ayla.GetScriptTypeDelegate s_GetScriptType__Delegate = () => typeof({classFullName});");
        m_SourceCode += m_Parent.IndentedLine($"private static nint GetScriptType__Invoke() => global::System.Runtime.InteropServices.Marshal.GetFunctionPointerForDelegate(s_GetScriptType__Delegate);");

        // Constructors
        for (int i = 0; i < m_Class.Constructors.Count; ++i)
        {
            GenerateInvocableConstructor(i, @class, injectFullName);
        }

        m_SourceCode += m_Parent.IndentedLine("");

        // Virtual functions
        GenerateFunctionBodies(true);
    }

    private void GenerateInvocableConstructor(int index, ClassName @class, string injectFullName)
    {
        var constructor = m_Class.Constructors[index];
        var returnType = (SharedPtrTypeName)Activator.CreateInstance(typeof(SharedPtrTypeName), @class)!;
        var parameters = CollectParameters(constructor.Parameters);
        var csharpParamsDeclare = ParametersGenerator.GenerateCSharp(parameters);
        var callArguments = FunctionBodyGenerator.GeneratePassArguments(parameters.AddFirstTemp(TypeName.IntPtr, "__gchandle_ptr"));
        
        m_SourceCode += m_Parent.IndentedLine($"protected {m_Class.Class.Name}__Invocable({csharpParamsDeclare}) : this(@this =>");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            var codegen = new FunctionBodyGenerator(
                parameters.AddFirstTemp(PlaceholderName.Value, "(nint)global::System.Runtime.InteropServices.GCHandle.Alloc(@this, global::System.Runtime.InteropServices.GCHandleType.Weak)"),
                $"{injectFullName}.ctor_{constructor.Name}",
                TypeName.Object
            );
            codegen.GenerateCSharpCSharpToNative(ref m_SourceCode, ref m_Parent.IndentRef, m_Parent.IndentedLine);
        });
        m_SourceCode += m_Parent.IndentedLine("})");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_SourceCode += m_Parent.IndentedLine("}");
    }

    private void GenerateMainClass(ClassName @class, string invocableFullName, string injectFullName)
    {
        m_SourceCode += m_Parent.IndentedLine($"public partial class {@class.Name} : {invocableFullName}, global::Ayla.IStaticObject");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            GenerateMainClassMembers(@class, injectFullName);
        });
        m_SourceCode += m_Parent.IndentedLine("}");
    }

    private void GenerateMainClassMembers(ClassName @class, string injectFullName)
    {
        m_SourceCode += "#pragma warning disable CS8618\n";

        // Protected constructor
        m_SourceCode += m_Parent.IndentedLine($"protected {@class.Name}(global::System.Func<object, nint> locker) : base(locker)");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_SourceCode += m_Parent.IndentedLine($"OnConstructed(locker);");
        });
        m_SourceCode += m_Parent.IndentedLine("}");
        m_SourceCode += "#pragma warning restore CS8618\n";
        m_SourceCode += m_Parent.IndentedLine($"partial void OnConstructed(global::System.Func<object, nint> locker);");
        m_SourceCode += m_Parent.IndentedLine("");

        // Public constructors
        for (int i = 0; i < m_Class.Constructors.Count; ++i)
        {
            GenerateMainClassConstructor(i);
        }

        m_SourceCode += m_Parent.IndentedLine("");
        m_SourceCode += m_Parent.IndentedLine($"public override global::Ayla.ManagedTypeWrapper GetClass() => StaticClass();");
        m_SourceCode += m_Parent.IndentedLine("");
        m_SourceCode += m_Parent.IndentedLine($"public static new global::Ayla.ManagedTypeWrapper StaticClass() => {injectFullName}.GetManagedType();");

        // Non-virtual functions
        GenerateFunctionBodies(false);
    }

    private void GenerateMainClassConstructor(int index)
    {
        var constructor = m_Class.Constructors[index];
        var access = constructor.Access.ToString().ToLower();
        var parameters = CollectParameters(constructor.Parameters);
        var csharpParamsDeclare = ParametersGenerator.GenerateCSharp(parameters);
        var callArguments = FunctionBodyGenerator.GeneratePassArguments(parameters);

        m_SourceCode += m_Parent.IndentedLine($"{access} {constructor.Name}({csharpParamsDeclare}) : base({callArguments})");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_SourceCode += m_Parent.IndentedLine($"OnConstructed({callArguments});");
        });
        m_SourceCode += m_Parent.IndentedLine("}");
        m_SourceCode += m_Parent.IndentedLine($"partial void OnConstructed({csharpParamsDeclare});");
    }

    private void GenerateFunctionBodies(bool acceptVirtual)
    {
        var @class = m_Parent.TypeNames.FindClass(m_Class.Class);
        var injectFullName = $"global::{string.Join(".", @class.Namespace.Names)}.{@class.Name}__Injected";

        for (int i = 0; i < m_Class.Functions.Count; ++i)
        {
            var function = m_Class.Functions[i];
            bool isVirtual = function.Flags.HasFlag(SFunction.FFlags.Virtual);

            if (isVirtual != acceptVirtual)
            {
                continue;
            }

            var access = function.Access.ToString().ToLower();
            var returnType = m_Parent.TypeNames.FindType(function.ReturnType, m_Class.Class);
            var parameterTypes = function.Parameters.Select(p => m_Parent.TypeNames.FindType(p.Variable.TypeName, m_Class.Class)).ToArray();
            bool isStatic = function.Flags.HasFlag(SFunction.FFlags.Static);
            var parameters = CollectParameters(function.Parameters);

            var internalParamsDeclare = string.Join(", ", parameterTypes.Select((t, j) => $"{t.CSharpName} {function.Parameters[j].Variable.Name}"));
            var returnStmt = returnType is SharedPtrTypeName ? $"{returnType.CSharpName}" : returnType.CSharpName;
            
            m_SourceCode += m_Parent.IndentedLine($"{access} unsafe{(isVirtual ? " virtual" : string.Empty)}{(isStatic ? " static" : string.Empty)} {returnStmt} {function.Name}({internalParamsDeclare})");
            m_SourceCode += m_Parent.IndentedLine("{");
            m_Parent.Indented(() =>
            {
                GenerateFunctionBody(function, returnType, parameters, isStatic, injectFullName);
            });
            m_SourceCode += m_Parent.IndentedLine("}");

            if (isVirtual)
            {
                GenerateVirtualInvokeMethod(function, returnType, parameters);
            }

            m_SourceCode += m_Parent.IndentedLine("");
        }
    }

    private void GenerateFunctionBody(SFunction function, TypeName returnType, ParameterCollection parameters, bool isStatic, string injectFullName)
    {
        var rule = m_Parent.Project.GetRule(m_Parent.BuildTarget);
        if (rule.Type == ModuleType.Application || rule.Type == ModuleType.Console)
        {
            m_SourceCode += m_Parent.IndentedLine($"throw new global::System.AccessViolationException(\"Assemblies of the Application or Console type cannot directly invoke native functions.\");");
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

        bodyGen.GenerateCSharpCSharpToNative(ref m_SourceCode, ref m_Parent.IndentRef, m_Parent.IndentedLine);
    }

    private void GenerateVirtualInvokeMethod(SFunction function, TypeName returnType, ParameterCollection parameters)
    {
        var invokeParamsDeclare = ParametersGenerator.GenerateCSharpBindings(parameters.AddFirstTemp(TypeName.Object, "self_"));
        m_SourceCode += m_Parent.IndentedLine($"private static unsafe {returnType.CSharpBindingName} {function.Name}__Invoke({invokeParamsDeclare})");
        m_SourceCode += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_SourceCode += m_Parent.IndentedLine($"var self = self_.AsManaged<{m_Class.Class.Name}>()!;");
            string callable = $"self.{function.Name}";
            var codeGen = new FunctionBodyGenerator(parameters, callable, returnType);
            codeGen.GenerateCSharpNativeToCSharp(ref m_SourceCode, ref m_Parent.IndentRef, m_Parent.IndentedLine);
        });
        m_SourceCode += m_Parent.IndentedLine("}");
    }

    private ParameterCollection CollectParameters(IEnumerable<SParameter> parameters)
    {
        var collection = new ParameterCollection();
        foreach (var param in parameters)
        {
            var paramType = m_Parent.TypeNames.FindType(param.Variable.TypeName, m_Class.Class);
            collection.Add(paramType, param.Variable.Name);
        }
        return collection;
    }
}
