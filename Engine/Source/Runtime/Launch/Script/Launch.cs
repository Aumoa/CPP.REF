namespace Ayla;

public partial class Launch
{
    private Engine m_Engine;

    public override void StartApplication()
    {
        m_Engine = new Engine();
        m_Engine.GuardedStartup();
        m_Engine.GuardedLoop();
    }
}
