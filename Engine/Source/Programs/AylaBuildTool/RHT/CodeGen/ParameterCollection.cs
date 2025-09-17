using static AylaEngine.RHT.IParameterCollection;

namespace AylaEngine.RHT.CodeGen;

internal class ParameterCollection : IParameterCollection
{
    private readonly List<ParameterInfo> m_Params;

    public ParameterCollection()
    {
        m_Params = [];
        Parameters = m_Params;
    }

    public IEnumerable<ParameterInfo> Parameters { get; }

    public int Count => m_Params.Count;

    public void Add(TypeName typeName, string name)
    {
        m_Params.Add(new ParameterInfo(typeName, name));
    }

    public ParametersAfterFirst AddFirstTemp(TypeName firstTypeName, string firstName) => new(Parameters, firstTypeName, firstName);
}
