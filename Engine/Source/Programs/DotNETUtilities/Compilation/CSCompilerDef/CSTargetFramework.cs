namespace AylaEngine;

public enum CSTargetFramework
{
    Net0900,
}

public static class CSTargetFrameworkExtensions
{
    public static string ToFrameworkString(this CSTargetFramework framework) => framework switch
    {
        CSTargetFramework.Net0900 => "net9.0",
        _ => throw new ArgumentOutOfRangeException(nameof(framework), framework, null)
    };

    public static CSTargetFramework Parse(string framework) => framework switch
    {
        "net9.0" => CSTargetFramework.Net0900,
        _ => throw new FormatException($"Unknown target framework '{framework}'.")
    };
}