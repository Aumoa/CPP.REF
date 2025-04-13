// Copyright 2020-2025 AylaEngine. All Rights Reserved.

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

        private TypeName(string cpp, string csharp, string? @namespace)
        {
            Cpp = cpp;
            CSharp = csharp;
            IsVoid = csharp == "void";
            Namespace = @namespace;
        }

        private static TypeName Void = new TypeName("void", "void", null);
        private static TypeName Boolean = new TypeName("bool", "bool", null);
        private static TypeName Int32 = new TypeName("::Ayla::int32", "int", null);
        private static TypeName Int64 = new TypeName("::Ayla::int64", "long", null);
        private static TypeName UInt32 = new TypeName("::Ayla::uint32", "uint", null);
        private static TypeName UInt64 = new TypeName("::Ayla::uint64", "ulong", null);
        private static TypeName IntPtr = new TypeName("::Ayla::ssize_t", "nint", null);
        private static TypeName UIntPtr = new TypeName("::Ayla::size_t", "unint", null);
        private static TypeName String = new TypeName("::Ayla::String", "string", null);
        private static TypeName Single = new TypeName("float", "float", null);
        private static TypeName Double = new TypeName("double", "double", null);

        public static TypeName FromNative(Context context, string typeName)
        {
            if (typeName.Contains("RPtr<") || typeName.Contains("PPtr<"))
            {
                var items = typeName.Split(['<', '>'], StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                var rptrTypeName = items[1];
                var scope = rptrTypeName.Split("::", StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
                if (scope.Length == 1)
                {
                    return new TypeName(scope[0], scope[0], null);
                }
                else
                {
                    return new TypeName(scope[1], scope[1], scope[0]);
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
