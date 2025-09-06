namespace Ayla;

public partial class Launch : IDisposable
{
    public void Dispose()
    {
        GC.SuppressFinalize(this);
    }

    public void StartApplication()
    {
        StartApplication_Injected(InstanceId);
    }

    public static Launch CreateInstance(string[] args) => CreateInstance_Internal(args) ?? throw new InvalidOperationException();
}
