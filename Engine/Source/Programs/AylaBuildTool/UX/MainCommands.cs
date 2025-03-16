
namespace AylaEngine;

internal class MainCommands : Commands
{
    private readonly MenuBuilder m_Builder;

    public MainCommands()
    {
        m_Builder = new MenuBuilder("Main");
        m_Builder.Add("Generate", OnGenerateAsync);
        m_Builder.Add("Build", OnBuildAsync);
    }

    public override async ValueTask RenderPromptAsync(CancellationToken cancellationToken)
    {
        await m_Builder.RenderPromptAsync(cancellationToken);
    }

    private static ValueTask OnGenerateAsync(CancellationToken cancellationToken)
    {
        Commands.Push<GenerateCommands>();
        return ValueTask.CompletedTask;
    }

    private static ValueTask OnBuildAsync(CancellationToken cancellationToken)
    {
        return ValueTask.CompletedTask;
    }
}
