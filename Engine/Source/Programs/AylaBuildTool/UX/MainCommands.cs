
namespace AylaEngine;

internal class MainCommands : Commands
{
    private readonly MenuBuilder m_Builder;

    public MainCommands()
    {
        m_Builder = new MenuBuilder("Main");
        m_Builder.Add("Generate", OnGenerateAsync);
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
}
