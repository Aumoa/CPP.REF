namespace AylaEngine;

internal partial class RHTGenerator
{
    private readonly Syntax[] m_Syntaxes;
    private readonly string m_FileId;

    public SourceCodeDescriptor SourceCode { get; }

    public SAClass[] Classes { get; }

    public SAEnum[] Enums { get; }

    internal Syntax[] Syntaxes => m_Syntaxes;

    private RHTGenerator(SourceCodeDescriptor sourceCode, Syntax[] syntaxes)
    {
        SourceCode = sourceCode;
        m_Syntaxes = syntaxes;
        Classes = syntaxes.OfType<SAClass>().ToArray();
        Enums = syntaxes.OfType<SAEnum>().ToArray();
        var sourceRelativePath = Path.GetRelativePath(SourceCode.Group.SourceDirectory, SourceCode.FilePath);
        m_FileId = sourceRelativePath
            .Replace('/', '_')
            .Replace('\\', '_')
            .Replace('.', '_')
            .ToUpper();
    }

    public static async Task<RHTGenerator?> ParseAsync(SourceCodeDescriptor sourceCode, CancellationToken cancellationToken = default)
    {
        var headerFileName = Path.GetFileNameWithoutExtension(sourceCode.FilePath);
        var headerText = await File.ReadAllTextAsync(sourceCode.FilePath, cancellationToken);
        var includeText = $"#include \"{headerFileName}.gen.h\"";
        int includeIndex = headerText.IndexOf(includeText);
        if (includeIndex == -1)
        {
            return null;
        }

        // #include must be lowercase.
        if (headerText.AsSpan(includeIndex, "#include".Length).Equals("#include", StringComparison.Ordinal) == false)
        {
            return null;
        }

        List<Syntax> syntaxes = [];
        List<Syntax> bracketStack = [];

        var context = new Context(sourceCode.FilePath, headerText);
        for (; context.IsEOF == false;)
        {
            if (bracketStack.LastOrDefault()?.EscapeBracket == context.CurrentChar)
            {
                bracketStack.RemoveAt(bracketStack.Count - 1);
                context.Advance(1);
                continue;
            }

            if (SComment.TryAccept(context, out var comment))
            {
                syntaxes.Add(comment);
            }
            else if (SInclude.TryAccept(context, out var include))
            {
                syntaxes.Add(include);
            }
            else if (SDefineLogCategory.TryAccept(context, bracketStack, out var defineLogCategory))
            {
                syntaxes.Add(defineLogCategory);
            }
            else if (SNamespace.TryAccept(context, out var @namespace))
            {
                syntaxes.Add(@namespace);
            }
            else if (SClass.TryAccept(context, bracketStack, out var @class))
            {
                syntaxes.Add(@class);
            }
            else if (SAClass.TryAccept(context, bracketStack, out var aclass))
            {
                syntaxes.Add(aclass);
            }
            else if (SGeneratedBody.TryAccept(context, out var generatedBody))
            {
                aclass = syntaxes.OfType<SAClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw generatedBody.Context.ParsingError("Syntax Error: A class definition is required before GENERATED_BODY().");
                }
                aclass.Body = generatedBody;
                continue;
            }
            else if (SProperty.TryAccept(context, syntaxes, out var aproperty))
            {
                aclass = syntaxes.OfType<SAClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw aproperty.Context.ParsingError("Syntax Error: A class definition is required before APROPERTY().");
                }
                aclass.AddProperty(aproperty);
                continue;
            }
            else if (SFunction.TryAccept(context, syntaxes, out var afunction))
            {
                aclass = syntaxes.OfType<SAClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw afunction.Context.ParsingError("Syntax Error: A class definition is required before AFUNCTION().");
                }
                aclass.AddFunction(afunction);
                continue;
            }
            else if (SConstructor.TryAccept(context, syntaxes, out var aconstructor))
            {
                aclass = syntaxes.OfType<SAClass>().LastOrDefault();
                if (aclass == null)
                {
                    throw aconstructor.Context.ParsingError("Syntax Error: A class definition is required before ACONSTRUCTOR().");
                }
                if (aclass.Class.Name != aconstructor.Name)
                {
                    throw aconstructor.Context.ParsingError($"Syntax Error: The constructor name '{aconstructor.Name}' does not match the class name '{aclass.Class.Name}'.");
                }
                aclass.AddConstructor(aconstructor);
                continue;
            }
            else if (SAEnum.TryAccept(context, bracketStack, out var aenum))
            {
                syntaxes.Add(aenum);
            }
            else if (SBracket.TryAccept(context, out var unknownBracket))
            {
                syntaxes.Add(unknownBracket);
            }
            else if (SAccessSpecifier.TryAccept(context, out var accessSpecifier))
            {
                syntaxes.Add(accessSpecifier);
            }
            else
            {
                context.Advance(1);
                continue;
            }

            if (syntaxes.Last().EscapeBracket.HasValue)
            {
                bracketStack.Add(syntaxes.Last());
            }
        }

        if (bracketStack.Count > 0)
        {
            throw bracketStack.Last().Context.ParsingError("Syntax Error: Unmatched brackets.");
        }

        var lastInclude = syntaxes.LastOrDefault(p => p is SInclude);
        if (lastInclude == null)
        {
            context.ParsingError("The mandatory header file must be included at the very end of the inclusion list.");
        }

        return new RHTGenerator(sourceCode, syntaxes.ToArray());
    }
}
