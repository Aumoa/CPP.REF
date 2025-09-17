namespace AylaEngine.RHT.CodeGen;

internal static class ParametersGenerator
{
    public static string GenerateCSharpInjected<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CSharpBindingName} {p.N}"));
    }

    public static string GenerateCSharp<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CSharpName} {p.N}"));
    }

    public static string GenerateCSharpInvoke<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CSharpBindingName} {p.N}"));
    }
}
