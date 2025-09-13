using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

public partial class Engine
{
    public Engine()
    {
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
        ConstructorInfo? primaryConstructor = AppDomain.CurrentDomain.GetAssemblies().SelectMany(p => p.GetTypes())
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
