using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

public partial class Engine
{
    private LaunchOptions m_Options;

    private static GetScriptTypeDelegate s_Get = () => typeof(Engine);

    public Engine(LaunchOptions options)
    {
        m_Options = options;
    }

    protected override void Dispose(bool disposing)
    {
        if (disposing)
        {
            Shutdown();
        }

        base.Dispose(disposing);
    }

    protected override Graphics InitializeGraphics()
    {
        return Graphics.CreateGraphics(RenderFeatures.Vulkan);
    }

    protected override GameInstance InitializeGameInstance()
    {
        var gameAssemblyName = m_Options.GetGameAssemblyName();
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
        LogEngine.Info("GameInstance created: {0}", gameInstance.GetType().FullName);
        gameInstance.SetEngine(this);
        return gameInstance;
    }
}
