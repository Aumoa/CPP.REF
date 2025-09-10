using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

public partial class Engine
{
    private readonly Assembly m_GameAssembly;

    private Engine(Assembly gameAssembly)
    {
        m_GameAssembly = gameAssembly;
    }

    public static ObjectReferenceWrapper CreateByNative([MarshalAs(UnmanagedType.LPWStr)] string gameAssemblyPath)
    {
        var assembly = Assembly.Load(Path.GetFileNameWithoutExtension(gameAssemblyPath) + ".Script");
        return new Engine(assembly).AsWrapper();
    }

    public override void GuardedStartup()
    {
        try
        {
            base.GuardedStartup();
        }
        catch (Exception e)
        {
            Debug.LogCritical("Engine", "Unhandled exception during startup: {0}", e);
        }
    }

    public override void GuardedLoop()
    {
        try
        {
            base.GuardedLoop();
        }
        catch (Exception e)
        {
            Debug.LogCritical("Engine", "Unhandled exception during loop: {0}", e);
        }
    }

    public override void Initialize()
    {
        base.Initialize();

        try
        {
            InitializeActivity();
            InitializeGraphics();
            InitializeGame();
            PostInitialized();
        }
        catch
        {
            Shutdown();
            throw;
        }
    }

    private GameInstance? m_GameInstance;

    private void InitializeGame()
    {
        ConstructorInfo? primaryConstructor = m_GameAssembly.GetTypes()
            .Where(p => p.IsAssignableTo(typeof(GameInstance)))
            .Select(p => p.GetConstructor(BindingFlags.Public, []))
            .FirstOrDefault(p => p != null);
        if (primaryConstructor == null)
        {
            Debug.LogCritical("Engine", "No public constructor found for GameInstance derived class.");
            return;
        }

        m_GameInstance = (GameInstance)primaryConstructor.Invoke([]);
        Debug.LogInformation("Engine", "GameInstance created: {0}", m_GameInstance.GetType().FullName);
    }
}
