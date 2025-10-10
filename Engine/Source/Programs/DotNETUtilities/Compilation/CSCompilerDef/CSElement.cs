using System.Text;

namespace AylaEngine;

public abstract record CSElement
{
    public abstract string GenerateXml(string? csprojPath);

    protected static string IndentLines(string s)
    {
        return string.Join('\n', s.Split('\n').Select(l => "  " + l));
    }
}
