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
}
