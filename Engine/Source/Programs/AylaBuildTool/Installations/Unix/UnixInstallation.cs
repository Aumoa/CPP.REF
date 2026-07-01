using System.Text;

namespace AylaEngine;

internal abstract class UnixInstallation : Installation
{
    public override async ValueTask<string[]> ParseDependenciesAsync(string depsFileName, CancellationToken cancellationToken)
    {
        var plain = await File.ReadAllTextAsync(depsFileName, cancellationToken);
        return ParseMakeRuleDependencies(plain);
    }

    internal static string[] ParseMakeRuleDependencies(string plain)
    {
        var dependencies = new List<string>();
        foreach (var line in FoldMakeContinuations(plain).Split('\n'))
        {
            var colonIndex = FindFirstUnescapedColon(line);
            if (colonIndex < 0)
            {
                continue;
            }

            ParseMakeTokens(line.AsSpan(colonIndex + 1), dependencies);
        }

        return dependencies.ToArray();
    }

    private static string FoldMakeContinuations(string plain)
    {
        var folded = new StringBuilder(plain.Length);
        for (var index = 0; index < plain.Length; ++index)
        {
            var current = plain[index];
            if (current == '\\')
            {
                if (index + 1 < plain.Length && plain[index + 1] == '\n')
                {
                    ++index;
                    continue;
                }

                if (index + 2 < plain.Length && plain[index + 1] == '\r' && plain[index + 2] == '\n')
                {
                    index += 2;
                    continue;
                }
            }

            if (current != '\r')
            {
                folded.Append(current);
            }
        }

        return folded.ToString();
    }

    private static int FindFirstUnescapedColon(string line)
    {
        var escaped = false;
        for (var index = 0; index < line.Length; ++index)
        {
            var current = line[index];
            if (escaped)
            {
                escaped = false;
                continue;
            }

            if (current == '\\')
            {
                escaped = true;
                continue;
            }

            if (current == ':')
            {
                return index;
            }
        }

        return -1;
    }

    private static void ParseMakeTokens(ReadOnlySpan<char> text, List<string> tokens)
    {
        var token = new StringBuilder();
        var escaped = false;
        foreach (var current in text)
        {
            if (escaped)
            {
                token.Append(current);
                escaped = false;
                continue;
            }

            if (current == '\\')
            {
                escaped = true;
                continue;
            }

            if (current == '#')
            {
                break;
            }

            if (char.IsWhiteSpace(current))
            {
                FlushToken();
                continue;
            }

            token.Append(current);
        }

        if (escaped)
        {
            token.Append('\\');
        }

        FlushToken();

        void FlushToken()
        {
            if (token.Length > 0)
            {
                tokens.Add(token.ToString());
                token.Clear();
            }
        }
    }
}
