// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal static class SyntaxEnumeratorExtensions
{
    public static List<SyntaxCore>? EnumerateNext(this IEnumerator<SyntaxCore> enumerator, Func<SyntaxCore, bool>? predic, bool ignoreComment = false)
    {
        List<SyntaxCore> results = new();

        static bool IsComment(SyntaxCore value)
        {
            return value.Type == SyntaxType.Comment || value.Type == SyntaxType.CommentDoc;
        }

        for (; ; )
        {
            SyntaxCore sCore = enumerator.Current;
            if (!ignoreComment || !IsComment(sCore))
            {
                results.Add(sCore);
            }

            if (predic?.Invoke(sCore) == true)
            {
                enumerator.MoveNext();
                return results;
            }

            if (!enumerator.MoveNext())
            {
                break;
            }
        }

        return predic == null ? results : null;
    }

    public static string? GetIdentifier(this IEnumerator<SyntaxCore> syntaxes, string? start = null)
    {
        if (start == null)
        {
            if (syntaxes.Current.Type != SyntaxType.Identifier)
            {
                return null;
            }

            start = syntaxes.Current.Name;
            syntaxes.MoveNext();
        }

        int level = 0;
        string composed = start;
        bool allowIdentifier = false;
        while (true)
        {
            if (syntaxes.Current.Name == ":")
            {
                if (!syntaxes.MoveNext())
                {
                    return null;
                }

                if (syntaxes.Current.Name != ":")
                {
                    return null;
                }

                composed += "::";
                syntaxes.MoveNext();
                allowIdentifier = true;
                continue;
            }
            else if (syntaxes.Current.Name == "<")
            {
                ++level;
                composed += '<';
                syntaxes.MoveNext();
                allowIdentifier = true;
                continue;
            }
            else if (syntaxes.Current.Name == ">")
            {
                --level;
                composed += '>';
                syntaxes.MoveNext();
                continue;
            }
            else
            {
                if (syntaxes.Current.Type == SyntaxType.Identifier && allowIdentifier)
                {
                    composed += syntaxes.Current.Name;
                    syntaxes.MoveNext();
                    allowIdentifier = false;
                    continue;
                }
                else if (syntaxes.Current.Type == SyntaxType.Keyword)
                {
                    if (syntaxes.Current.Name == "const" || syntaxes.Current.Name == "typename")
                    {
                        composed += ' ' + syntaxes.Current.Name + ' ';
                        syntaxes.MoveNext();
                        continue;
                    }
                }
                else if (syntaxes.Current.Type == SyntaxType.Operator)
                {
                    if (syntaxes.Current.Name == "*" || syntaxes.Current.Name == "&")
                    {
                        composed += syntaxes.Current.Name;
                        syntaxes.MoveNext();
                        allowIdentifier = false;
                        continue;
                    }
                }
                else
                {
                    if (level == 0)
                    {
                        return composed;
                    }
                    else
                    {
                        // error.
                        return null;
                    }
                }
            }
        }
    }

    public static List<SyntaxCore> GetScope(this IEnumerator<SyntaxCore> syntaxes, string open, string close)
    {
        int level = 0;
        List<SyntaxCore> list = new();
        if (syntaxes.Current.Name == "<")
        {
            while (true)
            {
                list.Add(syntaxes.Current);
                if (syntaxes.Current.Name == open)
                {
                    ++level;
                }
                else if (syntaxes.Current.Name == close)
                {
                    if (--level <= 0)
                    {
                        syntaxes.MoveNext();
                        break;
                    }
                }

                syntaxes.MoveNext();
            }
        }
        return list;
    }
}
