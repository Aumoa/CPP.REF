using System.Diagnostics.CodeAnalysis;

namespace AylaEngine;

internal record STypeName(CapturedContext Context, STypeName.Part[] NameParts) : Syntax(Context, null)
{
    public enum PartType
    {
        NamespaceSeparator,
        Name,
        TemplateBegin,
        TemplateEnd,
        TemplateSeparator,
        Number
    }

    public record struct Part(PartType Type, string? Name);

    public string FullName
    {
        get
        {
            return string.Concat(NameParts.Select(p => p.Type switch
            {
                PartType.NamespaceSeparator => "::",
                PartType.Name => p.Name,
                PartType.TemplateBegin => "<",
                PartType.TemplateEnd => ">",
                PartType.TemplateSeparator => ", ",
                PartType.Number => p.Name,
                _ => throw TerminateException.Internal()
            }));
        }
    }

    public static STypeName Accept(Context context)
    {
        var capture = context.Capture();
        char cc = context.CurrentChar;
        if (IsNumber(cc))
        {
            throw capture.ParsingError("Syntax Error: A type name cannot start with a number.");
        }

        List<Part> parts = [];

        while (context.IsEOF == false)
        {
            cc = context.CurrentChar;
            if (cc == ':' && context.Current.StartsWith("::"))
            {
                if (NotSame(PartType.NamespaceSeparator) == false)
                {
                    throw capture.ParsingError("Syntax Error: Unexpected '::'.");
                }

                parts.Add(new Part(PartType.NamespaceSeparator, null));
                context.WholeAdvance("::");
                continue;
            }

            if (cc == '<')
            {
                if (Required(PartType.Name) == false)
                {
                    throw capture.ParsingError("Syntax Error: Unexpected '<'.");
                }

                parts.Add(new Part(PartType.TemplateBegin, null));
                context.WholeAdvance("<");
                continue;
            }

            if (cc == ',')
            {
                if (Required(PartType.Name) == false)
                {
                    throw capture.ParsingError("Syntax Error: Unexpected '<'.");
                }

                parts.Add(new Part(PartType.TemplateSeparator, null));
                context.WholeAdvance(",");
                continue;
            }

            if (cc == '>')
            {
                if (Required(PartType.Name, PartType.TemplateBegin, PartType.Number, PartType.TemplateEnd) == false)
                {
                    throw capture.ParsingError("Syntax Error: Unexpected '>'.");
                }

                parts.Add(new Part(PartType.TemplateEnd, null));
                context.WholeAdvance(">");

                // If the number of TemplateBegin and TemplateEnd are the same, the parsing is complete.
                if (TemplateDepth() == 0)
                {
                    break;
                }

                continue;
            }

            if (IsNumber(cc))
            {
                if (Required(PartType.TemplateBegin, PartType.TemplateSeparator) == false)
                {
                    throw capture.ParsingError("Syntax Error: Unexpected number.");
                }

                var number = context.ExportWhile(c => IsNumber(c) || c == '\'');
                if (number[^1] == '\'')
                {
                    throw capture.ParsingError("Syntax Error: A number cannot end with a single quote (').");
                }

                parts.Add(new Part(PartType.Number, number.ToString().Replace("'", string.Empty)));
                continue;
            }

            if (IsNameChar(cc))
            {
                if (NotSame(PartType.Name, PartType.TemplateSeparator, PartType.Number, PartType.TemplateEnd) == false)
                {
                    if (Required(PartType.Name) && TemplateDepth() == 0)
                    {
                        break;
                    }

                    throw capture.ParsingError("Syntax Error: Unexpected name.");
                }

                var name = context.ExportWhile(c => IsNameChar(c) || IsNumber(c));
                parts.Add(new Part(PartType.Name, name.ToString()));
                continue;
            }

            break;
        }

        if (parts.Any(p => p.Type == PartType.Name) == false)
        {
            throw capture.ParsingError("Syntax Error: A type name is required.");
        }

        return new STypeName(capture, parts.ToArray());

        bool IsNumber(char c) => c >= '0' && c <= '9';
        bool IsNameChar(char c) => (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || IsNumber(c) || c == '_';
        int TemplateDepth() => parts.Count(p => p.Type == PartType.TemplateBegin) - parts.Count(p => p.Type == PartType.TemplateEnd);

        bool Required(params ReadOnlySpan<PartType> partTypes)
        {
            if (parts.Count == 0)
            {
                return false;
            }

            foreach (var item in partTypes)
            {
                if (parts.Last().Type == item)
                {
                    return true;
                }
            }

            return false;
        }

        bool NotSame(params ReadOnlySpan<PartType> partTypes)
        {
            if (parts.Count == 0)
            {
                return true;
            }

            foreach (var item in partTypes)
            {
                if (parts.Last().Type == item)
                {
                    return false;
                }
            }

            return true;
        }
    }
}
