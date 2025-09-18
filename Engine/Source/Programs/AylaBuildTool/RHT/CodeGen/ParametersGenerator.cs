namespace AylaEngine.RHT.CodeGen;

internal static class ParametersGenerator
{
    public static string GenerateCSharp<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CSharpName} {p.N}"));
    }

    public static string GenerateCSharpBindings<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CSharpBindingName} {p.N}"));
    }

    public static string GenerateCpp<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CppName} {p.N}"));
    }

    public static string GenerateCppBindings<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CppBindingName} {p.N}"));
    }
}
