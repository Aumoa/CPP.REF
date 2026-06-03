namespace AylaEngine;

internal readonly record struct ProjectCandidate(
    ProjectCandidateKind Kind,
    string Name,
    GroupDescriptor Group,
    string SourceDirectory,
    string DefinitionFileName);
