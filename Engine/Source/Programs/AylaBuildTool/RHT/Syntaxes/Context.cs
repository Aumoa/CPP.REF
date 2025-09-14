namespace AylaEngine;

internal class Context : ICapturedContext
{
    private string m_FullText;
    private int m_Index;
    private bool m_Freezed;

    public string FilePath { get; }

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

    public CapturedContext Capture()
    {
        return new CapturedContext(FilePath, LineNumber, ColumnNumber);
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

        throw this.ParsingError("Syntax Error: Expected closing escape character.");
    }

    public ReadOnlySpan<char> ExportWhile(Predicate<char> predicate)
    {
        int start = m_Index;
        for (int i = start; i < m_FullText.Length; ++i)
        {
            if (predicate(m_FullText[i]) == false)
            {
                var result = m_FullText.AsSpan(start, i - start);
                Advance(result.Length);
                return ReplaceNewLine(result);
            }
        }

        var finalResult = m_FullText.AsSpan(start, m_FullText.Length - start);
        Advance(finalResult.Length);
        return ReplaceNewLine(finalResult);
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

    private static ReadOnlySpan<char> ReplaceNewLine(ReadOnlySpan<char> source)
    {
        if (source.Length == 0)
        {
            return source;
        }

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
