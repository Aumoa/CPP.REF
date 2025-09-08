namespace Ayla;

public partial class Launch
{
    public void StartApplication()
    {
        StartApplication_Internal();
    }

    public static Launch CreateInstance(string[] args) => CreateInstance_Internal(args) ?? throw new InvalidOperationException();
}
