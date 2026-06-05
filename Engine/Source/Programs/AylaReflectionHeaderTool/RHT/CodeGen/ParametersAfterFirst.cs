using static AylaEngine.RHT.IParameterCollection;

namespace AylaEngine.RHT.CodeGen;

internal struct ParametersAfterFirst(IEnumerable<ParameterInfo> parameters, TypeName firstTypeName, string firstName) : IParameterCollection
{
    private ParameterInfo[] m_Parameters = [new ParameterInfo(firstTypeName, firstName), .. parameters];

    public IEnumerable<ParameterInfo> Parameters => m_Parameters;

    public int Count => m_Parameters.Length;
}
