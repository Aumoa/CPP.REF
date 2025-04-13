namespace AylaEngine;

internal partial class RHTGenerator
{
    private class ParsingErrorException : Exception
    {
        public ParsingErrorException(string filePath, int lineNumber, int columnNumber, ReadOnlySpan<char> message)
            : base($"{filePath}({lineNumber},{columnNumber}): {message}")
        {
        }
    }

    internal class Context
    {
        public readonly string FilePath;
        private readonly string m_FullText;
        private int m_Index;
        private bool m_Freezed;

        public int LineNumber { get; private set; } = 1;

        public int ColumnNumber { get; private set; } = 0;

        private Context(string filePath, string fullText, int index, bool freezed, int lineNumber, int columnNumber)
        {
            FilePath = filePath;
            m_FullText = fullText;
            m_Index = index;
            m_Freezed = freezed;
            LineNumber = lineNumber;
            ColumnNumber = columnNumber;
        }

        public Context(string filePath, string fullText)
        {
            FilePath = filePath;
            m_FullText = fullText;
        }

        public Context Capture()
        {
            return new Context(FilePath, m_FullText, m_Index, true, LineNumber, ColumnNumber);
        }

        public ReadOnlySpan<char> Current => m_FullText.AsSpan(m_Index);

        public char CurrentChar => m_Index < m_FullText.Length ? m_FullText[m_Index] : '\0';

        public bool IsEOF => m_Index >= m_FullText.Length;

        public bool WholeEquals(ReadOnlySpan<char> value)
        {
            return Current.StartsWith(value) && (m_Index + value.Length >= m_FullText.Length || char.IsWhiteSpace(m_FullText[m_Index + value.Length]));
        }

        public bool Advance(int advance)
        {
            if (m_Freezed)
            {
                throw TerminateException.Internal();
            }

            for (int i = m_Index; i < m_FullText.Length && advance > 0;)
            {
                --advance;
                if (++m_Index >= m_FullText.Length)
                {
                    return false;
                }

                var ch = m_FullText[m_Index];
                if (ch == '\n')
                {
                    ColumnNumber = 0;
                    ++LineNumber;
                }
                else if (ch == '\r')
                {
                    continue;
                }
                else
                {
                    ++ColumnNumber;
                }
            }

            return true;
        }

        public bool WholeAdvance(ReadOnlySpan<char> value)
        {
            Advance(value.Length);
            for (; IsEOF == false;)
            {
                if (char.IsWhiteSpace(CurrentChar))
                {
                    Advance(1);
                }
                else
                {
                    break;
                }
            }

            return IsEOF == false;
        }

        public void SkipWhiteSpace(bool allowNewLine)
        {
            while (m_Index < m_FullText.Length)
            {
                var ch = m_FullText[m_Index];
                if (char.IsWhiteSpace(ch) && (allowNewLine || (ch != '\n' && ch != '\r')))
                {
                    Advance(1);
                    continue;
                }

                break;
            }
        }

        public ReadOnlySpan<char> Export(int enter, ReadOnlySpan<char> escape)
        {
            int start = m_Index + enter;
            for (int i = start; i < m_FullText.Length; ++i)
            {
                if (m_FullText.AsSpan(i).StartsWith(escape))
                {
                    var result = m_FullText.AsSpan(start, i - start);
                    Advance(result.Length + escape.Length);
                    return ReplaceNewLine(result);
                }
            }

            throw ParsingError("Syntax Error: Expected closing escape character.");
        }

        public int SelectExport(int enter, params string[] values)
        {
            int start = m_Index + enter;
            for (int i = start; i < m_FullText.Length; ++i)
            {
                for (int j = 0; j < values.Length; ++j)
                {
                    if (m_FullText.AsSpan(i).StartsWith(values[j]))
                    {
                        return j;
                    }
                }
            }

            return -1;
        }

        public Exception ParsingError(ReadOnlySpan<char> message)
        {
            return new ParsingErrorException(FilePath, LineNumber, ColumnNumber, message);
        }

        private static ReadOnlySpan<char> ReplaceNewLine(ReadOnlySpan<char> source)
        {
            if (source[0] == '\n')
            {
                source = source[1..];
            }
            
            if (source[^1] == '\r')
            {
                source = source[..^1];
            }

            if (source.Contains("\r\n", StringComparison.Ordinal))
            {
                source = source.ToString().Replace("\r\n", "\n").AsSpan();
            }

            return source;
        }
    }

    internal abstract class Syntax
    {
        public readonly Context Context;

        public int LineNumber => Context.LineNumber;

        public int ColumnNumber => Context.ColumnNumber;

        protected Syntax(Context context)
        {
            Context = context;
        }

        public abstract char? EscapeBracket { get; }

        protected string FormatLineNumber()
        {
            return $"({LineNumber},{ColumnNumber}): ";
        }
    }

    private readonly Syntax[] m_Syntaxes;
    private readonly string m_FileId;

    public SourceCodeDescriptor SourceCode { get; }

    public AClass[] Classes { get; }

    private RHTGenerator(SourceCodeDescriptor sourceCode, Syntax[] syntaxes)
    {
        SourceCode = sourceCode;
        m_Syntaxes = syntaxes;
        Classes = syntaxes.OfType<AClass>().ToArray();
        var sourceRelativePath = Path.GetRelativePath(SourceCode.Group.SourceDirectory, SourceCode.FilePath);
        m_FileId = sourceRelativePath
            .Replace('/', '_')
            .Replace('\\', '_')
            .Replace('.', '_')
            .ToUpper();
    }

    private static string FunctionName(string @namespace, string @class, string name)
    {
        return $"Injected__{@namespace}__{@class}__{name}";
    }

    public static async Task<RHTGenerator?> ParseAsync(SourceCodeDescriptor sourceCode, CancellationToken cancellationToken = default)
    {
        var headerFileName = Path.GetFileNameWithoutExtension(sourceCode.FilePath);
        var headerText = await File.ReadAllTextAsync(sourceCode.FilePath, cancellationToken);
        var includeText = $"#include \"{headerFileName}.gen.h\"";
        int includeIndex = headerText.IndexOf(includeText);
        if (includeIndex == -1)
        {
            return null;
        }

        // #include must be lowercase.
        if (headerText.AsSpan(includeIndex, "#include".Length).Equals("#include", StringComparison.Ordinal) == false)
        {
            return null;
        }

        List<Syntax> syntaxes = [];
        List<Syntax> bracketStack = [];

        var context = new Context(sourceCode.FilePath, headerText);
        for (; context.IsEOF == false;)
        {
            if (bracketStack.LastOrDefault()?.EscapeBracket == context.CurrentChar)
            {
                bracketStack.RemoveAt(bracketStack.Count - 1);
                context.Advance(1);
                continue;
            }

            if (Comment.TryAccept(context, out var comment))
            {
                syntaxes.Add(comment);
            }
            else if (Include.TryAccept(context, out var include))
            {
                syntaxes.Add(include);
            }
            else if (Namespace.TryAccept(context, out var @namespace))
            {
                syntaxes.Add(@namespace);
            }
            else if (Class.TryAccept(context, bracketStack, out var @class))
            {
                syntaxes.Add(@class);
            }
            else if (AClass.TryAccept(context, bracketStack, out var aclass))
            {
                syntaxes.Add(aclass);
            }
            else if (GeneratedBody.TryAccept(context, out var generatedBody))
            {
                aclass = syntaxes.OfType<AClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw generatedBody.Context.ParsingError("Syntax Error: A class definition is required before GENERATED_BODY().");
                }
                aclass.Body = generatedBody;
                continue;
            }
            else if (AProperty.TryAccept(context, out var aproperty))
            {
                aclass = syntaxes.OfType<AClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw aproperty.Context.ParsingError("Syntax Error: A class definition is required before APROPERTY().");
                }
                aclass.AddProperty(aproperty);
                continue;
            }
            else if (AFunction.TryAccept(context, out var afunction))
            {
                aclass = syntaxes.OfType<AClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw afunction.Context.ParsingError("Syntax Error: A class definition is required before AFUNCTION().");
                }
                aclass.AddFunction(afunction);
                continue;
            }
            else if (UnknownBracket.TryAccept(context, out var unknownBracket))
            {
                syntaxes.Add(unknownBracket);
            }
            else
            {
                context.Advance(1);
                continue;
            }

            if (syntaxes.Last().EscapeBracket.HasValue)
            {
                bracketStack.Add(syntaxes.Last());
            }
        }

        if (bracketStack.Count > 0)
        {
            throw bracketStack.Last().Context.ParsingError("Syntax Error: Unmatched brackets.");
        }

        var lastInclude = syntaxes.LastOrDefault(p => p is Include);
        if (lastInclude == null)
        {
            context.ParsingError("The mandatory header file must be included at the very end of the inclusion list.");
        }

        return new RHTGenerator(sourceCode, syntaxes.ToArray());
    }
}
