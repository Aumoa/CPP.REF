// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine.RHT.CodeGen;

/// <summary>
/// Generator for C++ enum header definitions.
/// </summary>
internal class HeaderEnumGenerator
{
    private readonly HeaderGenerator m_Parent;
    private readonly SAEnum m_Enum;

    public HeaderEnumGenerator(HeaderGenerator parent, SAEnum aenum)
    {
        m_Parent = parent;
        m_Enum = aenum;
    }

    public void Generate()
    {
        string cppnamespace = string.Join("::", m_Enum.Namespaces.Select(p => p.Name));
        string fullname = "::" + string.Join("::", m_Enum.Namespaces.Select(p => p.Name).Append(m_Enum.Name));
        string functionName = $"{cppnamespace.Replace("::", "__")}__{m_Enum.Name}__ToString";

        // Forward declaration
        m_Parent.HeaderText += m_Parent.IndentedLine($"namespace {cppnamespace}");
        m_Parent.HeaderText += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_Parent.HeaderText += m_Parent.IndentedLine($"enum class {m_Enum.Name};");
        });
        m_Parent.HeaderText += m_Parent.IndentedLine("}");
        m_Parent.HeaderText += m_Parent.IndentedLine("");
        
        // ToString function declaration
        m_Parent.HeaderText += m_Parent.IndentedLine($"PLATFORM_SHARED_EXPORT ::Ayla::String {functionName}({fullname} value);");
        m_Parent.HeaderText += m_Parent.IndentedLine("");

        // Formatter specialization
        m_Parent.HeaderText += m_Parent.IndentedLine($"template<class TChar> requires (::std::same_as<TChar, char> || ::std::same_as<TChar, wchar_t>)");
        m_Parent.HeaderText += m_Parent.IndentedLine($"struct std::formatter<{fullname}, TChar> : public ::std::formatter<::Ayla::String, TChar>");
        m_Parent.HeaderText += m_Parent.IndentedLine("{");
        m_Parent.Indented(() =>
        {
            m_Parent.HeaderText += m_Parent.IndentedLine($"template<class TFormatContext>");
            m_Parent.HeaderText += m_Parent.IndentedLine($"auto format({fullname} value, TFormatContext& ctx) const");
            m_Parent.HeaderText += m_Parent.IndentedLine("{");
            m_Parent.Indented(() =>
            {
                m_Parent.HeaderText += m_Parent.IndentedLine($"return ::std::formatter<::Ayla::String, TChar>::format({functionName}(value), ctx);");
            });
            m_Parent.HeaderText += m_Parent.IndentedLine("}");
        });
        m_Parent.HeaderText += m_Parent.IndentedLine("};");
    }
}
