namespace Ayla;

public partial class Launch : IDisposable
{
    public Launch()
    {
    }

    public void Dispose()
    {
        GC.SuppressFinalize(this);
    }

    public void StartApplication()
    {
        StartApplication_Injected(0);
    }
}
