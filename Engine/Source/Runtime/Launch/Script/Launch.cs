using System.Reflection;

namespace Ayla;

public partial class Launch
{
    private GenericApplication m_App;

    public override int GuardedMain(LaunchOptions options)
    {
        LoadPlatformAssembly(options.GetPlatformAssemblyName());
        using (var engine = new Engine(options))
        {
            engine.Initialize();
            engine.GuardedLoop();
        }
        return 0;
    }

    private void LoadPlatformAssembly(string platform)
    {
        var assembly = Assembly.Load(platform + ".Script");
        var constructor = assembly.GetTypes()
            .Where(p => p.IsAbstract == false && p.IsAssignableTo(typeof(GenericApplication)))
            .Select(p => p.GetConstructor([]))
            .FirstOrDefault();
        if (constructor == null)
        {
            throw new InvalidOperationException("No non-abstract GenericApplication constructor found in the platform assembly.");
        }
        
        m_App = (GenericApplication)constructor.Invoke([]);
    }
}
