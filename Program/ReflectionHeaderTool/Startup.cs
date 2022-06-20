// Copyright 2020-2022 Aumoa.lib. All right reserved.

using DotNETUtilities;

using System.Diagnostics;
using System.Text;

namespace ReflectionHeaderTool;

internal class Startup
{
    public static void Main(string[] args)
    {
        var commandLine = CommandLine.FromConsoleArgs(args);

        DirectoryReference sourceDir;
        if (!commandLine.TryGetValue('s', out var value))
        {
            throw new Exception("Source directory does not specified. Pass -s argument for specify source directory.");
        }

        sourceDir = new DirectoryReference(value);

        DirectoryReference outDir;
        if (!commandLine.TryGetValue('o', out value))
        {
            throw new Exception("Output directory does not specified. Pass -o argument for specify output directory.");
        }

        outDir = new DirectoryReference(value);

        FileReference[] headerFiles = sourceDir.GetAllFiles(true).Where(p => p.CompareExtension(".h")).ToArray();

        Console.WriteLine("Reflection Header Tool for {0} project.", sourceDir.Name);
        Stopwatch timer = Stopwatch.StartNew();

#if PRINT_SYNTAX
        headerFiles = new[]
        {
            new FileReference(@"F:\CPP.REF\Engine\Source\Runtime\Core\Public\Object.h")
        };
#endif

        List<Task> tasks = new();
        foreach (var headerFile in headerFiles)
        {
            var file = headerFile;
            var fileKey = file.GetRelativePath(sourceDir)
                .Replace(Path.DirectorySeparatorChar, '_')
                .Replace('.', '_');

            tasks.Add(Task.Run(() =>
            {
                SyntaxTree sTree = new(file);
                sTree.Parse();

                StringBuilder sbHeader = new();
                StringBuilder sbSource = new();
                var compile = new CppCompilation(sTree, fileKey);
                if (!compile.Emit(sbHeader, sbSource))
                {
                    return;
                }

                FileReference generatedHeader = outDir.GetFile(headerFile.ChangeExtensions(".generated.h").FileName);
                if (generatedHeader.WriteAllTextIfChanged(sbHeader.ToString(), Encoding.UTF8))
                {
                    Console.WriteLine("{0}", generatedHeader);
                }

                FileReference generatedSource = outDir.GetFile(headerFile.ChangeExtensions(".gen.cpp").FileName);
                if (generatedSource.WriteAllTextIfChanged(sbSource.ToString(), Encoding.UTF8))
                {
                    Console.WriteLine("{0}", generatedSource);
                }
            }));
        }

        Task.WhenAll(tasks).GetAwaiter().GetResult();

        timer.Stop();
        Console.WriteLine("Reflection header generated and {0} seconds elapsed.", timer.Elapsed.TotalSeconds);
    }
}
