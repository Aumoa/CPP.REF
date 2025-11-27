using System.Reflection;

namespace Ayla;

public partial class Graphics
{
    public static Graphics CreateGraphics(RenderFeatures api)
    {
        string assemblyName = api switch
        {
            RenderFeatures.Vulkan => "VulkanAPI.Script",
            RenderFeatures.D3D12 => "Direct3D12.Script",
            _ => throw new NotSupportedException($"The specified Render API '{api}' is not supported.")
        };

        var graphicsAssembly = Assembly.Load(assemblyName);
        var graphicsType = graphicsAssembly
            .GetTypes()
            .FirstOrDefault(t => t.IsAbstract == false && t.IsAssignableTo(typeof(Graphics)));
        if (graphicsType == null)
        {
            throw new InvalidOperationException($"No type found in assembly '{assemblyName}' that derives from Graphics.");
        }

        var ctor = graphicsType.GetConstructor(Type.EmptyTypes);
        if (ctor == null)
        {
            throw new InvalidOperationException($"The type '{graphicsType.FullName}' does not have a parameterless constructor.");
        }

        return (Graphics)ctor.Invoke([]);
    }
}
