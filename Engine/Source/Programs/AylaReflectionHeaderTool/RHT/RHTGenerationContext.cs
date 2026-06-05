namespace AylaEngine.RHT;

public readonly record struct RHTGenerationContext(
    string ScriptAssemblyName,
    bool AllowsNativeFunctionInvocation);
