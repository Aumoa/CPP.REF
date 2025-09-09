using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

public partial class Engine
{
    private readonly Assembly m_GameAssembly;

    private Engine(Assembly gameAssembly) : this(Engine__Internal())
    {
        m_GameAssembly = gameAssembly;
    }

    public static ObjectReferenceWrapper CreateByNative([MarshalAs(UnmanagedType.LPWStr)] string gameAssemblyPath)
    {
        var assembly = Assembly.Load(Path.GetFileNameWithoutExtension(gameAssemblyPath) + ".Script");
        return new Engine(assembly).AsWrapper();
    }
}
