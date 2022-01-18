// Copyright 2020-2021 Aumoa.lib. All right reserved.

using CodeProjectConfiguration;

using DotNETUtilities;

using System.Reflection;

namespace BuildTool;

class CompiledModuleCode<T> where T : class
{
    public readonly FileReference RuleCode;
    public readonly Type RuleClass;
    public readonly T CompiledRule;

    public CompiledModuleCode(FileReference CodeFile)
    {
        if (!CodeFile.IsExist)
        {
            throw new FileNotFoundException();
        }

        RuleCode = CodeFile;

        Assembly CompiledCodeAssembly;
        try
        {
            CompiledCodeAssembly = RuntimeCompilation.CompileAssembly(
                AssemblyName: CodeFile.FileName,
                SourceFiles: new() { CodeFile },
                ReferencedAssemblies: new()
                {
                    new FileReference(typeof(object).Assembly.Location),
                    new FileReference(typeof(ModuleRule).Assembly.Location)
                }
            );
        }
        catch (Exception E)
        {
            throw new RuleClassCompileErrorException($"An exception occurred while compiling rule code. Error: {E.Message}");
        }

        List<Type> Subclasses = new();
        foreach (Type Type in CompiledCodeAssembly.GetTypes())
        {
            if (Type.IsSubclassOf(typeof(T)))
            {
                Subclasses.Add(Type);
            }
        }

        if (Subclasses.Count <= 0)
        {
            throw new RuleClassNotFoundException("Couldn't find rule class. There is no classes that derived from 'CodeProjectConfiguration.SolutionRule' class.");
        }

        Type RuleClass = Subclasses[0];
        if (Subclasses.Count > 1)
        {
            Console.WriteLine("Warning: There are two more classes that inherit by 'CodeProjectConfiguration.SolutionRule' class. Select zero index class by default[{0}]", RuleClass.Name);
        }

        this.RuleClass = RuleClass;
        CompiledRule = Activator.CreateInstance(RuleClass) as T;
    }
}