using Spectre.Console;

namespace AylaEngine;

internal class MenuBuilder(string title)
{
    private record MenuItem
    {
        public required string Name { get; init; }
        public required int Number { get; init; }
        public required Func<CancellationToken, ValueTask> Action { get; init; }

        public override string ToString() => $"{Number,2}. {Name}";
    }

    private readonly List<MenuItem> m_Items = new();

    public async ValueTask RenderPromptAsync(CancellationToken cancellationToken)
    {
        IEnumerable<MenuItem> items;
        if (HasReturn)
        {
            items = m_Items.Append(new MenuItem { Name = Commands.IsMain ? "Exit" : "Return", Number = 0, Action = OnReturn });
        }
        else
        {
            items = m_Items;
        }

        var result = await new SelectionPrompt<MenuItem>()
            .Title(title)
            .AddChoices(items)
            .ShowAsync(AnsiConsole.Console, cancellationToken);
        await result.Action(cancellationToken);
    }

    public bool HasReturn { get; init; } = true;

    public void Add(string name, Func<CancellationToken, ValueTask> action)
    {
        m_Items.Add(new MenuItem { Name = name, Number = m_Items.Count + 1, Action = action });
    }

    private static ValueTask OnReturn(CancellationToken _)
    {
        Commands.Pop();
        return ValueTask.CompletedTask;
    }
}
