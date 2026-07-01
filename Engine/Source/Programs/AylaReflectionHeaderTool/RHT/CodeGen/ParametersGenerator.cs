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

    public static string GenerateCSharpNativeToManagedBindings<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CSharpNativeToManagedBindingName} {p.N}"));
    }

    public static string GenerateCSharpManagedToNativeBindings<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CSharpManagedToNativeBindingName} {p.N}"));
    }

    public static string GenerateCpp<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CppName} {p.N}"));
    }

    public static string GenerateCppBindings<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CppBindingName} {p.N}"));
    }

    public static string GenerateCppNativeToManagedBindings<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CppNativeToManagedBindingName} {p.N}"));
    }

    public static string GenerateCppManagedToNativeBindings<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => $"{p.T.CppManagedToNativeBindingName} {p.N}"));
    }

    public static string GenerateCppParametersOnly<T>(T collection) where T : IParameterCollection
    {
        return string.Join(", ", collection.Parameters.Select(p => p.T.CppName));
    }
}
