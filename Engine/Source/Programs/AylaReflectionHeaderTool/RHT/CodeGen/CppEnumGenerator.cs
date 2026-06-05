// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C++ enum ToString function.
/// </summary>
internal class CppEnumGenerator
{
    private readonly CppSourceGenerator m_Parent;
    private readonly SAEnum m_Enum;

    public CppEnumGenerator(CppSourceGenerator parent, SAEnum aenum)
    {
        m_Parent = parent;
        m_Enum = aenum;
    }

    public void Generate()
    {
        string cppnamespace = string.Join("__", m_Enum.Namespaces.Select(n => n.Name));
        string fullname = "::" + string.Join("::", m_Enum.Namespaces.Select(p => p.Name).Append(m_Enum.Name));
        string functionName = $"{cppnamespace.Replace("::", "__")}__{m_Enum.Name}__ToString";

        m_Parent.WriteIndentedLine($"PLATFORM_SHARED_EXPORT ::Ayla::String {functionName}({fullname} value)");
        m_Parent.WriteIndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_Parent.WriteIndentedLine($"switch (value)");
            m_Parent.WriteIndentedLine("{");
            m_Parent.Indented(() =>
            {
                foreach (var define in m_Enum.Defines)
                {
                    m_Parent.WriteIndentedLine($"case {fullname}::{define.Name}:");
                    m_Parent.Indented(() =>
                    {
                        m_Parent.WriteIndentedLine($"return TEXT(\"{define.Name}\");");
                    });
                }

                m_Parent.WriteIndentedLine($"default:");
                m_Parent.Indented(() =>
                {
                    m_Parent.WriteIndentedLine($"return ::Ayla::String::Format(TEXT(\"{m_Enum.Name}({{}})\"), (int)value);");
                });
            });
            m_Parent.WriteIndentedLine("}");
        });
        m_Parent.WriteIndentedLine("}");
    }
}
