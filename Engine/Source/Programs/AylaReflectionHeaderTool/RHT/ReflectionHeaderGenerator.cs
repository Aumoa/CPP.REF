namespace AylaEngine.RHT;

public static class ReflectionHeaderGenerator
{
    public static async Task<RHTGenerationResult> GenerateAsync(IEnumerable<RHTGenerationInput> inputs, CancellationToken cancellationToken = default)
    {
        var inputArray = inputs.ToArray();
        var parsedSources = await Task.WhenAll(inputArray.Select(input => ParseAsync(input, cancellationToken)));
        var errorTexts = parsedSources
            .Where(parsedSource => parsedSource.ErrorText != null)
            .Select(parsedSource => parsedSource.ErrorText!)
            .ToArray();

        if (errorTexts.Length > 0)
        {
            return new RHTGenerationResult([], errorTexts);
        }

        var typeNames = CreateTypeNames(parsedSources.Select(parsedSource => parsedSource.Generator).OfType<RHTGenerator>());
        var generatedSources = parsedSources
            .Where(parsedSource => parsedSource.Generator != null)
            .Select(parsedSource => GenerateSource(parsedSource.Input, parsedSource.Generator!, typeNames))
            .ToArray();

        return new RHTGenerationResult(generatedSources, []);
    }

    private static async Task<ParsedRHTSource> ParseAsync(RHTGenerationInput input, CancellationToken cancellationToken)
    {
        try
        {
            var generator = await RHTGenerator.ParseAsync(input.SourceFile, cancellationToken);
            return new ParsedRHTSource(input, generator, null);
        }
        catch (Exception e)
        {
            return new ParsedRHTSource(input, null, e.Message);
        }
    }

    private static RHTGeneratedSource GenerateSource(RHTGenerationInput input, RHTGenerator generator, TypeNames typeNames)
    {
        return new RHTGeneratedSource(
            input,
            generator.GenerateHeader(typeNames),
            generator.GenerateSourceCode(input.Context, typeNames),
            generator.GenerateCSharp(input.Context, typeNames));
    }

    private static TypeNames CreateTypeNames(IEnumerable<RHTGenerator> generators)
    {
        List<TypeName> typeNames = [];

        foreach (var generator in generators)
        {
            foreach (var @class in generator.Classes)
            {
                typeNames.Add(new ClassName(
                    new NamespaceName(@class.Class.Namespaces.Select(p => p.Name).ToArray()),
                    @class.Class.Name,
                    generator));
            }

            foreach (var @enum in generator.Enums)
            {
                typeNames.Add(new EnumName(
                    new NamespaceName(@enum.Namespaces.Select(p => p.Name).ToArray()),
                    @enum.Name,
                    generator));
            }
        }

        return new TypeNames(typeNames.ToArray());
    }

    private readonly record struct ParsedRHTSource(
        RHTGenerationInput Input,
        RHTGenerator? Generator,
        string? ErrorText);
}
