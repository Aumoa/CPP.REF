namespace AylaEngine;

internal sealed class BuildPlan
{
    private readonly BuildAction[] m_Actions;

    public BuildPlan(IEnumerable<BuildAction> actions)
    {
        m_Actions = actions.ToArray();
    }

    public IReadOnlyList<BuildAction> Actions => m_Actions;

    public Task ExecuteAsync(CancellationToken cancellationToken)
    {
        return new BuildActionExecutor(m_Actions).ExecuteAsync(cancellationToken);
    }
}
