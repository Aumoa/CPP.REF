// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public class MakefileContext
{
    public List<MakefileCompile> CompileNodes { get; } = new();

    public List<MakefileLink> LinkNodes { get; } = new();

    public int CompiledCount = 0;

    public int TotalCount { get; private set; }

    public string TotalCountStr { get; private set; } = string.Empty;

    public static MakefileContext Create(IEnumerable<Makefile> makefiles)
    {
        var context = new MakefileContext();

        foreach (var makefile in makefiles)
        {
            List<MakefileCompile> items = new();
            foreach (var item in makefile.MakefileCompiles)
            {
                if (item.IsRequired == false)
                {
                    item.MarkCompleted();
                    continue;
                }

                context.CompileNodes.Add(item);
                context.TotalCount++;
            }

            context.LinkNodes.Add(makefile.MakefileLink);
            context.TotalCount++;
        }

        context.TotalCountStr = context.TotalCount.ToString();
        return context;
    }
}
