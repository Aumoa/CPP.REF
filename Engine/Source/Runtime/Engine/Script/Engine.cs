using System.Reflection;
using System.Runtime.InteropServices;

namespace Ayla;

public partial class Engine
{
    public static void InitializeGame_Managed([MarshalAs(UnmanagedType.LPWStr)] string gameAssemblyPath)
    {
        var assembly = Assembly.Load(Path.GetFileNameWithoutExtension(gameAssemblyPath) + ".Script");
        var defaultGameInstance = assembly.GetTypes().Where(p => p.IsAssignableTo(typeof(GameInstance))).FirstOrDefault();
        if (defaultGameInstance == null)
        {
            throw new InvalidOperationException();
        }
    }
}
