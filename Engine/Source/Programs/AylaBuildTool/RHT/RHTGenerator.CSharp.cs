using AylaEngine.RHT.CodeGen;

namespace AylaEngine.RHT;

internal partial class RHTGenerator
{
    public string GenerateCSharp(ModuleProject project, TargetInfo buildTarget, TypeNames typeNames)
    {
        var generator = new CSharpCodeGenerator(this, project, buildTarget, typeNames);
        return generator.Generate();
    }
}
