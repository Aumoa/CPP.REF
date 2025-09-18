namespace Ayla;

public partial class Launch
{
    private Engine m_Engine;

    public override void StartApplication()
    {
        m_Engine = new Engine(0, [], "", [], null, []);
        m_Engine.Initialize();

        m_Engine.GuardedLoop();
    }
}
