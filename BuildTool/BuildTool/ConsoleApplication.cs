// Copyright 2020-2021 Aumoa.lib. All right reserved.

using DotNETUtilites;

using DotNETUtilities;

namespace BuildTool;

class ConsoleApplication
{
    CommandLine CommandLine;

    public ConsoleApplication(string[] Args)
    {
        CommandLine = new CommandLine(Args);
    }

    int Run()
    {
        int Hash = "BuildTool".GetHashCode();

        try
        {
            Console.WriteLine("BuildTool Arguments: {0}", string.Join(" ", CommandLine.OriginalArgs));

            if (!CommandLine.TryGetArgument("Solution", out var Args) || !(Args.Args?.Length >= 1))
            {
                throw new CommandLineArgumentException("'Solution' argument required.");
            }

            string Solution = Args.Args[0];
            try
            {
                string SolutionRoot = Directory.GetParent(Solution).FullName;
                Environment.CurrentDirectory = SolutionRoot;

                var Sln = new Solution(new FileReference(Solution));
                Sln.SearchProjects();

                string GeneratorClass;
                if (CommandLine.TryGetArgument("Generator", out Args) && Args.Args?.Length >= 1)
                {
                    GeneratorClass = Args.Args[0];
                }
                else if (Environment.OSVersion.Platform == PlatformID.Win32NT)
                {
                    GeneratorClass = "VisualStudioGenerator";
                }
                else
                {
                    GeneratorClass = "CMakeGenerator";
                }

                var Assembly = typeof(ConsoleApplication).Assembly;
                Type FoundType = Assembly.GetType("BuildTool." + GeneratorClass);
                if (FoundType == null)
                {
                    throw new GeneratorException($"Couldn't find generator class: {GeneratorClass}");
                }

                List<KeyValuePair<string, Type>> CtorParams = new();
                foreach (var Ctor in FoundType.GetConstructors())
                {
                    foreach (var Param in Ctor.GetParameters())
                    {
                        CtorParams.Add(new KeyValuePair<string, Type>(Param.Name, Param.ParameterType));
                    }

                    break;
                }

                List<object> CtorArgs = new();
                foreach (var (Name, Arg) in CtorParams)
                {
                    CommandLine.TryGetArgument(Name, out var CommandArg);

                    if (Arg.IsEnum && CommandArg.Args?.Length > 0)
                    {
                        Enum.TryParse(Arg, CommandArg.Args[0], out object Value);
                        CtorArgs.Add(Value);
                    }
                    else
                    {
                        CtorArgs.Add(null);
                    }
                }

                ISolutionGenerator Generator = null;
                Generator = Activator.CreateInstance(FoundType, CtorArgs.ToArray()) as ISolutionGenerator;
                if (Generator == null)
                {
                    throw new GeneratorException($"Couldn't find generator class: {GeneratorClass}");
                }

                IGeneratedSolution GeneratedSolution = Generator.Generate(Sln);
                GeneratedSolution.SaveAll();

                Console.WriteLine("Solution generated in '{0}'.", GeneratedSolution.SolutionFile.FullPath);
            }
            catch (FileNotFoundException E)
            {
                throw new CommandLineArgumentException(E.Message);
            }
        }
        catch (Exception E)
        {
            Console.WriteLine("Unexpected error! Exception: {0}", E);
            return E.HResult;
        }

        return 0;
    }

    static int Main(string[] Args)
    {
        return new ConsoleApplication(Args).Run();
    }
}