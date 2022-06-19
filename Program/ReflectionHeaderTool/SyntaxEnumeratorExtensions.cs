// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace ReflectionHeaderTool;

internal static class SyntaxEnumeratorExtensions
{
    public static List<SyntaxCore>? EnumerateNext(this IEnumerator<SyntaxCore> enumerator, Func<SyntaxCore, bool>? predic)
    {
        List<SyntaxCore> results = new();

        for (; ; )
        {
            SyntaxCore sCore = enumerator.Current;
            results.Add(sCore);

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
