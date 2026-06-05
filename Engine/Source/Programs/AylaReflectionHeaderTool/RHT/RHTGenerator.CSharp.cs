using AylaEngine.RHT.CodeGen;

namespace AylaEngine.RHT;

internal partial class RHTGenerator
{
    public string GenerateCSharp(RHTGenerationContext context, TypeNames typeNames)
    {
        var generator = new CSharpCodeGenerator(this, context, typeNames);
        return generator.Generate();
    }
}
