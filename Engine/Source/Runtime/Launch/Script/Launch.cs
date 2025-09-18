namespace Ayla;

public partial class Launch
{
    public override void StartApplication()
    {
        using (var engine = new Engine())
        {
            engine.Initialize();
            engine.GuardedLoop();
        }
    }
}
