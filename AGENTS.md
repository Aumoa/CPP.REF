# Repository Agent Rules

## Project Intent

- The project aims to combine suitable strengths from Unity and Unreal Engine.
- The project is primarily for learning and experimentation.

## Design Principles

- Prefer faithful object-oriented design by default, with clear responsibilities, encapsulation, and extension points.
- Performance-critical code may deliberately bend object-oriented design when doing so improves performance without significantly harming readability, maintainability, or local reasoning.
- Use Microsoft's recommended C# design guidelines and object-oriented patterns from major engines such as Unreal Engine as major references, adapting them to this project's goals rather than copying them mechanically.

## Code Style

- Repository files and generated documentation should be written in English.
- When creating or editing files, use the line ending appropriate for the current platform: CRLF on Windows and LF on Linux or macOS.
- C++ and C# member fields should use the `m_` prefix.
- Class names do not use engine-style prefixes, so member prefixes help reduce naming collisions.

## Build Rules

- Native C++ builds must be driven by AylaBuildTool.
- Do not use generated C++ IDE projects as normal build entry points. They may be used for IDE navigation or compile diagnostics, but AylaBuildTool owns the actual C++ build orchestration.
- Use the repository-local `ayla-build` skill at `.codex/skills/ayla-build/SKILL.md` for detailed build, generation, and verification workflows.

## Testing Policy

- When implementing features, actively add focused tests for deterministic or self-contained logic that is naturally testable, such as math utilities, parsers, serializers, build graph decisions, rule resolution, reflection metadata, interop marshalling, lifetime management, and state transformations.
- Use tests as self-validation for code that can be isolated without brittle infrastructure, GPU/runtime dependencies, or excessive setup.
- Before committing feature work with tests, run the relevant tests when practical and report the result.

## Documentation Policy

- Before changing behavior, architecture, build flow, interop contracts, or other documented systems, check the relevant files under `docs/`.
- When a change invalidates, extends, or clarifies existing documentation, update the relevant document in the same functional unit.
- Prefer documenting durable contracts, design intent, extension points, and verification steps that future maintainers or AI agents can reuse.

## Dependency Security Policy

- Before adding or using an external library, make a first-pass judgment that the library is trustworthy, maintained, appropriate for the repository, and aligned with local platform conventions.
- Prefer established libraries and avoid obscure dependencies when the standard library, platform SDK, or existing repository code can reasonably cover the need.
- When vulnerability or advisory information is available, check it before using a dependency and avoid known-vulnerable versions unless there is an explicit, documented reason and no safer practical alternative.

## Review Rules

- Use the repository-local `ayla-pr-review` skill at `.codex/skills/ayla-pr-review/SKILL.md` for pull request reviews, branch diff reviews, CI failure reviews, and merge-readiness checks.
- Before approving a pull request, understand the intent of the changed code. If the intent cannot be inferred from the source, tests, pull request description, or surrounding context, ask the pull request author to explain it and do not approve until that uncertainty is resolved.

## GitHub CLI And Shared-State Safety

- Codex may use the `gh` command for read-only GitHub inspection when it is available. Use the repository-local `ayla-build` and `ayla-pr-review` skills for detailed Actions validation workflows.
- Before choosing credentials for GitHub writes, remote branch publication, pull request creation or updates, author-side pull request responses, or remote validation, read and follow `.codex/skills/github-app-credential-policy/SKILL.md`.
- Codex may perform local-only actions at its own discretion, including read-only checks, local commits, and local merges.
- On shared working branches such as `dev`, `master`, `main`, release branches, or any branch that appears to be used directly by other people, Codex must request explicit user approval before any action that can affect other users, remote branches, hosted services, or shared state.
- On clearly isolated task branches, especially branches whose names start with `codex/`, Codex may perform non-destructive shared-state actions at its own discretion when they support the requested work. This includes pushing that branch, updating a pull request for that branch, or triggering GitHub Actions through that branch.
- Destructive or broad shared-state actions still require explicit user approval. This includes force-pushing, deleting remote branches or resources, publishing or deploying to shared environments, or changing shared working branches.
- If Codex is unsure whether a branch is isolated, it must treat the branch as shared and request approval before shared-state actions.
- Pull request review feedback, approvals, change requests, and independent code-evaluation comments must use the current ordinary GitHub user account. Do not use GitHub App, bot, or integration credentials for reviewer judgment.
- GitHub App or bot credentials should be used only for author-side pull request activity, such as replying to existing review feedback, explaining pushed commits, updating pull request descriptions, reporting validation results, or asking for re-review when requested.
- If a GitHub write has a required actor and receives a 401, 403, or permission error, do not retry with a different actor. Diagnose and report the credential or permission mismatch.
- Before merging a task branch into a protected shared branch, remove branch-local CI or GitHub Actions configuration changes that would affect the protected branch. Such temporary settings may exist on task branches, but they must be reverted or removed before approval and merge.
- Permanent CI or GitHub Actions policy changes, such as running compile checks for pull requests targeting `dev`, are normal repository policy changes and must not be treated as temporary branch-local validation settings.

## Instruction Storage

- Keep short, always-on repository policies in `AGENTS.md`.
- When a requested persistent instruction is mainly useful for a recurring workflow, verification procedure, tool usage pattern, or detailed domain guide, create or update a repository-local skill under `.codex/skills/<skill-name>/SKILL.md` and link it from `AGENTS.md`.
- Prefer scripts only for deterministic, repeatedly executed operations where a command or code snippet would otherwise be rewritten often.
- Do not split instructions just to split them. Keep compact universal rules in `AGENTS.md` when a skill or script would not reduce future context or execution risk.

## Commit Rules

- Codex AI must create a commit for each completed functional unit to keep history reviewable.
- Do not mix unrelated refactors, formatting, dependency updates, or bug fixes into the same commit unless they are required for that functional unit.
- Before committing, run the relevant build or test command when it is known and practical.
- If the relevant build or tests fail, do not commit until the failure is fixed or the user explicitly asks to commit anyway.
- Do not commit user-made unrelated changes. If the working tree already contains unrelated changes, isolate only Codex-made changes in the commit.
- Do not amend commits unless the user explicitly requests an amend; create a follow-up commit instead when prior commits may already be shared.
- Commits generated by Codex AI must use the `Codex: ` title prefix.
- AI-generated commit titles must be written in English.
- When practical, AI-generated commits should include a concise summary in the commit message body.
- For commits on isolated work branches intended to participate in a GitHub App-authenticated or bot-authored remote workflow, use the broker-provided App identity by default unless the user explicitly directs using an ordinary user account; follow `.codex/skills/github-app-credential-policy/SKILL.md` before creating the commit.
