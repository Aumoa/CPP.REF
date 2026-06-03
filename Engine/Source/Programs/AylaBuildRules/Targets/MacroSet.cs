namespace AylaEngine;

public readonly struct MacroSet
{
    public required string VarName { get; init; }

    public string? Value { get; init; }

    public static implicit operator MacroSet(string value)
    {
        var lr = value.Split('=');
        switch (lr.Length)
        {
            case 1:
                return new() { VarName = lr[0], Value = null };
            case 2:
                return new() { VarName = lr[0], Value = lr[1] };
            default:
                throw new FormatException();
        }
    }
}
