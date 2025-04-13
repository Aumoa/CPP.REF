// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Data.Common;
using System.Reflection.Metadata;
using static AylaEngine.RHTGenerator;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public struct TypeName
    {
        public readonly string Cpp;
        public readonly string CSharp;
        public readonly bool IsVoid;
        public readonly string? Namespace;
        public readonly bool ByRef;

        private TypeName(string cpp, string csharp, string? @namespace, bool byRef)
        {
            Cpp = cpp;
            CSharp = csharp;
            IsVoid = csharp == "void";
            Namespace = @namespace;
            ByRef = byRef;
        }

        private static TypeName Void = new TypeName("void", "void", null, false);
        private static TypeName Boolean = new TypeName("bool", "bool", null, false);
        private static TypeName Int32 = new TypeName("::Ayla::int32", "int", null, false);
        private static TypeName Int64 = new TypeName("::Ayla::int64", "long", null, false);
        private static TypeName UInt32 = new TypeName("::Ayla::uint32", "uint", null, false);
        private static TypeName UInt64 = new TypeName("::Ayla::uint64", "ulong", null, false);
        private static TypeName IntPtr = new TypeName("::Ayla::ssize_t", "nint", null, false);
        private static TypeName UIntPtr = new TypeName("::Ayla::size_t", "unint", null, false);
        private static TypeName String = new TypeName("::Ayla::String", "string", null, false);
        private static TypeName Single = new TypeName("float", "float", null, false);
        private static TypeName Double = new TypeName("double", "double", null, false);

        public readonly string CppFullName(AClass fromClass)
        {
            // If a namespace is specified along with the class declaration, the explicit namespace is prioritized.
            if (Namespace != null)
            {
                return "::" + Namespace + "::" + Cpp;
            }
            // Otherwise, it is initially assumed to be the same as the current class's namespace.
            else if (fromClass.Class.Namespace != "Ayla")
            {
                return "::" + fromClass.Class.Namespace + "::" + Cpp;
            }
            // Finally, it is assumed to be the engine namespace(Ayla).
            else
            {
                return "::Ayla::" + Cpp;
            }
        }

        public readonly string CSharpFullName(AClass fromClass)
        {
            // If a namespace is specified along with the class declaration, the explicit namespace is prioritized.
            if (Namespace != null)
            {
                return "global::" + Namespace + "." + Cpp;
            }
            // Otherwise, it is initially assumed to be the same as the current class's namespace.
            else if (fromClass.Class.Namespace != "Ayla")
            {
                return "global::" + fromClass.Class.Namespace + "." + Cpp;
            }
            // Finally, it is assumed to be the engine namespace(Ayla).
            else
            {
                return "global::Ayla." + Cpp;
            }
        }

        public readonly string CppBindingsParameter => ByRef ? "::Ayla::ssize_t" : Cpp;

        public string CppBindingsArgument(AClass fromClass, string name)
            => ByRef ? $"(::Ayla::Marshal::IntPtrToRPtr<{CppFullName(fromClass)}>({name}))" : name;

        public string CSharpBindingParameter(AClass fromClass)
            => ByRef ? CSharpFullName(fromClass) : CSharp;

        public string CSharpBindingArgument(string name)
            => ByRef ? $"global::Ayla.Marshal.GetInstancePointer({name})" : name;

        public string Injected
            => ByRef ? "nint" : CSharp;

        public static TypeName FromNative(Context context, string typeName)
        {
            if (typeName.Contains("RPtr<") || typeName.Contains("PPtr<"))
            {
                var items = typeName.Split(['<', '>'], StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                var rptrTypeName = items[1];
                var scope = rptrTypeName.Split("::", StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                if (scope.Length == 1)
                {
                    return new TypeName(scope[0], scope[0], null, true);
                }
                else
                {
                    return new TypeName(scope[1], scope[1], scope[0], true);
                }
            }

            return typeName switch
            {
                "void" => Void,
                "bool" => Boolean,
                "::Ayla::int32" or "Ayla::int32" or "int32" => Int32,
                "::Ayla::int64" or "Ayla::int64" or "int64" => Int64,
                "::Ayla::uint32" or "Ayla::uint32" or "uint32" => UInt32,
                "::Ayla::uint64" or "Ayla::uint64" or "uint64" => UInt64,
                "::Ayla::ssize_t" or "Ayla::ssize_t" or "ssize_t" => IntPtr,
                "::Ayla::size_t" or "Ayla::size_t" or "size_t" => UIntPtr,
                "::Ayla::String" or "Ayla::String" or "String" => String,
                "float" => Single,
                "double" => Double,
                _ => throw context.ParsingError($"Syntax Error: Unsupported typename '{typeName}'.")
            };
        }
    }
}
