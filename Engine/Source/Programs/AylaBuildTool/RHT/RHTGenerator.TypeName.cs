// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using static AylaEngine.RHTGenerator;

namespace AylaEngine;

internal partial class RHTGenerator
{
    public struct TypeName
    {
        public readonly string Cpp;
        public readonly string CSharp;

        private TypeName(string cpp, string csharp)
        {
            Cpp = cpp;
            CSharp = csharp;
        }

        private static TypeName Void = new TypeName("void", "void");
        private static TypeName Boolean = new TypeName("bool", "bool");
        private static TypeName Int32 = new TypeName("::Ayla::int32", "int");
        private static TypeName Int64 = new TypeName("::Ayla::int64", "long");
        private static TypeName UInt32 = new TypeName("::Ayla::uint32", "uint");
        private static TypeName UInt64 = new TypeName("::Ayla::uint64", "ulong");
        private static TypeName IntPtr = new TypeName("::Ayla::ssize_t", "nint");
        private static TypeName UIntPtr = new TypeName("::Ayla::size_t", "unint");
        private static TypeName String = new TypeName("::Ayla::String", "string");
        private static TypeName Single = new TypeName("float", "float");
        private static TypeName Double = new TypeName("double", "double");

        public static TypeName? FromNative(string typeName)
        {
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
                _ => null
            };
        }
    }
}
