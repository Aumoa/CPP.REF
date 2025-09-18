using System.Reflection;

namespace Ayla;

public partial class Launch
{
    private GenericApplication m_App;

    public override int GuardedMain(string platform)
    {
        LoadPlatformAssembly(platform);
        return 0;
    }

    private void LoadPlatformAssembly(string platform)
    {
        var assembly = Assembly.Load(platform + ".Script");
        var constructor = assembly.GetTypes()
            .Where(p => p.IsAssignableTo(typeof(GenericApplication)))
            .Select(p => p.GetConstructor([])!)
            .FirstOrDefault();
        if (constructor == null)
        {
            throw new InvalidOperationException();
        }
        
        m_App = (GenericApplication)constructor.Invoke([]);
    }
}
