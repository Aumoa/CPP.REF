using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

public partial class Engine
{
    private GenericActivity m_MainActivity = null!;
    private Graphics m_Graphics = null!;
    private List<GenericWindowSwapchainExtension> m_SwapchainExtensions = [];
    private GameInstance m_GameInstance;

    public Engine(LaunchOptions options)
    {
        m_MainActivity = GenericApplication.Get().CreateMainActivity();
        m_MainActivity.BeforeInitialize();

        m_Graphics = Graphics.CreateGraphics(RenderFeatures.Vulkan);
        m_SwapchainExtensions.Add(m_Graphics.InstallSwapChain(m_MainActivity.GetMainWindow()));

        m_GameInstance = InitializeGame(options);

        m_MainActivity.AfterInitialize();
        SetupSwapchainExtensions([.. m_SwapchainExtensions]);
    }

    protected override void Dispose(bool disposing)
    {
        if (disposing)
        {
            Shutdown();
        }

        base.Dispose(disposing);
    }

    public override void GuardedLoop()
    {
        while (true)
        {
            try
            {
                base.GuardedLoop();
                break;
            }
            catch (Exception e)
            {
                Debug.LogError("Engine", "Exception caught in main loop: {0}", e);
            }
        }
    }

    private GameInstance InitializeGame(LaunchOptions options)
    {
        var gameAssemblyName = options.GetGameAssemblyName();
        var gameAssembly = Assembly.Load(gameAssemblyName + ".Script");

        var gameInstanceType = gameAssembly.GetTypes()
            .Where(p => p.IsAbstract == false && p.IsAssignableTo(typeof(GameInstance)))
            .FirstOrDefault();
        if (gameInstanceType == null)
        {
            throw new InvalidOperationException("No GameInstance derived type found in loaded assemblies.");
        }

        var primaryConstructor = gameInstanceType.GetConstructor([]);
        if (primaryConstructor == null)
        {
            throw new InvalidOperationException("No public constructor found for GameInstance derived class.");
        }

        var gameInstance = (GameInstance)primaryConstructor.Invoke([]);
        Debug.LogInformation("Engine", "GameInstance created: {0}", gameInstance.GetType().FullName);
        return gameInstance;
    }
}
