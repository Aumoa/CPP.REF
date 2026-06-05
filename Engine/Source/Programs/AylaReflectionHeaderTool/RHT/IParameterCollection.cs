namespace AylaEngine.RHT;

internal interface IParameterCollection
{
    record struct ParameterInfo(TypeName T, string N);

    IEnumerable<ParameterInfo> Parameters { get; }
    int Count { get; }
}
