---
name: ayla-pr-review
description: Repository-local pull request review guidance for Ayla engine changes. Use when Codex reviews PRs, branch diffs, CI failures, architecture changes, API surface changes, class responsibilities, helper extraction, test coverage, dependencies, engine boundaries, or merge readiness in this repository.
---

# Ayla PR Review

## Overview

Ayla PR reviews should protect correctness, build health, and the repository's intended architecture without turning style preference into noise.

## Review Priorities

- Lead with actionable findings ordered by severity.
- Ground findings in concrete source locations, logs, commands, diffs, generated output, runtime behavior, or build graph behavior.
- Separate blockers from residual risks and optional follow-up ideas.
- Understand the author's intended behavior and design before approving a pull request.
- Infer intent from the source, tests, pull request description, names, structure, comments, and surrounding implementation.
- If the intent of changed code cannot be inferred from names, structure, comments, tests, pull request description, or surrounding implementation, ask the pull request author to explain it.
- Treat unresolved intent uncertainty as a merge-readiness blocker. Do not approve until the author explains the intent or the code is clarified enough to review its behavior.
- When reviewing CI or build changes, connect workflow edits back to the actual AylaBuildTool, module rules, generated files, and platform-specific code paths they exercise.

## API Surface

- Check whether newly added or widened `public` and `protected` C# members, exported C++ APIs, public headers, module rules, command-line options, and generated-code contracts are intentionally part of an API surface or extension point.
- Prefer the narrowest accessibility or exposure that supports the current design.
- Flag unnecessary exposure when the member, header, option, or exported symbol is only an implementation detail or can reasonably stay private/internal/module-local.
- Allow exposed APIs without current call sites when the code clearly defines an appropriate contract, override point, module boundary, tool command, or future extension point.
- Do not request narrower accessibility solely because a well-scoped API has no current in-repository caller.

## GitHub Actions

- Use `gh` to inspect Actions runs, jobs, and logs when the command is available.
- When `gh` is available, verify compile results for each supported platform in GitHub Actions: Windows, macOS, and Linux. Prefer job-level conclusions over only checking the overall workflow conclusion.
- If `gh` or Actions access is unavailable, state that Actions validation was skipped and perform a stricter source and workflow review for Windows, macOS, and Linux support.
- Before using GitHub credentials for remote validation, pull request updates, or bot-authored review responses, follow `.codex/skills/github-app-credential-policy/SKILL.md`.
- Local-only review actions, including read-only checks, local commits, and local merges, may be performed at Codex's discretion.
- On shared working branches such as `dev`, `master`, `main`, release branches, or branches that appear to be used directly by other people, request explicit user approval before shared-state actions.
- On clearly isolated task branches, especially branches whose names start with `codex/`, non-destructive shared-state actions may be performed at Codex's discretion when they support the review or validation work. This includes pushing that branch, updating its pull request, or triggering Actions through that branch.
- Destructive or broad shared-state actions still require explicit user approval. If branch ownership is unclear, treat the branch as shared.

## Merge Readiness

- Do not approve a PR while blockers remain unresolved, including unresolved intent uncertainty, missing practical tests for testable behavior, unresolved dependency risk, or incomplete platform validation for touched platform-sensitive code.
- Before approving or merging into protected shared branches such as `dev`, `master`, `main`, or release branches, check whether the PR includes CI, GitHub Actions, branch trigger, permission, environment, or workflow configuration changes that were only needed for task-branch validation.
- Temporary branch-local CI settings are allowed on isolated task branches, but they must be removed before the task branch is approved for merge into a protected shared branch.
- Do not block intentionally permanent CI policy changes, such as enabling pull request compile checks for protected branch targets, merely because they affect the protected branch after merge.
- Require a revert commit or a separate cleanup commit when temporary CI settings would otherwise affect the protected branch after merge.
- Treat unremoved temporary CI or Actions configuration as a merge blocker, even if the code changes themselves look correct.
- For documentation-only or instruction-only PRs, verify the instruction location, trigger, and persistence rather than running unrelated builds.

## Object-Oriented Design

- Prefer object-oriented designs with clear responsibilities, encapsulation, and extensibility unless the touched code is genuinely performance-critical.
- Accept performance-oriented departures from object-oriented design only when they do not significantly harm readability, maintainability, or local reasoning.
- Use Microsoft's recommended C# design guidelines and object-oriented patterns from major engines such as Unreal Engine as review references, adapted to Ayla's conventions.

## Class Responsibility

- Check whether each meaningful feature is owned by a clear class or collaborator.
- Flag classes that combine unrelated responsibilities when that coupling makes behavior harder to extend, test, or review.
- Prefer changes that move behavior into cohesive feature units rather than scattered special cases.
- Do not request extra splitting when the existing responsibility boundary is already understandable and further separation would mostly add indirection.
- Do not ask authors to undo harmless fine-grained separation solely because it is more detailed than necessary.

## Function Extraction

- Review helper-method extraction by the meaning of the behavior, not by line count alone.
- Avoid asking for one-off helpers for trivial one- or two-line behavior when the extracted method has no distinct engine, build, platform, or domain meaning and is only used in one fixed location.
- Prefer extraction when the helper represents a clear feature unit, policy, validation rule, build step, reflection step, platform boundary, lifetime rule, or reusable decision.
- Prefer extraction when a behavior change should naturally apply to every call site through one shared implementation.
- Flag over-extraction when it obscures local flow, hides important context, or creates names that merely restate the code.
- Flag under-extraction when repeated or conceptually distinct logic makes the caller harder to understand or risks inconsistent future changes.

## Duplication

- Flag duplicated code when it represents the same feature, policy, decision, or platform behavior and merging it would reduce bug risk or future maintenance.
- Prefer one cohesive implementation for repeated behavior that must evolve together.
- Avoid demanding abstractions for small incidental duplication when the abstraction would be noisier than the repeated code.

## Engine Boundary And Safety

- Prioritize issues that can corrupt object lifetime, cross C++/C# ownership boundaries incorrectly, expose invalid generated metadata, execute unintended build steps, load files from unintended locations, or let one runtime/platform backend make assumptions that another backend does not satisfy.
- Treat native/managed interop, weak/strong reference handoff, reflection metadata, shader and asset loading paths, build-tool command execution, and platform-specific code paths as safety-sensitive.
- Flag script-, project-, or user-provided type names, paths, module names, capabilities, or command arguments when they are treated as authoritative without validation at the engine/build boundary.
- For renderer, platform, and build-system changes, verify that the changed behavior preserves the intended ownership boundary between modules and does not introduce hidden dependencies on unrelated modules, generated projects, local SDKs, or developer-machine state.

## Test Coverage Review

- Check whether the PR reports relevant test results in the description, review discussion, CI, or validation notes.
- Treat missing focused tests as a merge-readiness blocker when changed behavior is practical to cover with tests, especially math utilities, parsers, serializers, build graph decisions, module rule resolution, reflection metadata, interop marshalling, lifetime management, validators, deterministic state transformations, and regression-prone edge cases.
- Require the author to add tests and report the test result when testable behavior lacks coverage.
- Accept a no-new-test path only when the change is not practical to isolate, is mostly mechanical, or is better validated through integration/runtime checks; require that rationale and validation result to be stated.
- Prefer focused tests that exercise the changed behavior directly before relying only on broad runtime smoke checks.

## Dependency Review

- Check newly added or updated external libraries, SDK dependencies, NuGet packages, native libraries, and toolchain assumptions for trustworthiness, maintenance, real-world adoption, platform fit, and fit with existing repository conventions.
- For NuGet dependency changes, require vulnerability/advisory information to be checked before approval when that information is available. Prefer `dotnet list package --vulnerable --include-transitive` or equivalent advisory evidence when practical.
- Treat known-vulnerable dependency versions as merge-readiness blockers unless the PR documents why the version is necessary, why safer versions are not practical, and what mitigation or follow-up exists.
- Do not block a dependency solely because no vulnerability data is available, but require stronger evidence that the dependency is established and maintained before accepting it.
- Ask the author to report dependency trust and vulnerability-check results when a PR adds or updates external dependencies without that evidence.

## Review Comments

- Write external PR review comments in English unless the user asks otherwise.
- Explain the interpretation and recommendation to the user in Korean when the surrounding conversation is Korean.
- Clearly state whether the reviewed change is safe to merge, needs fixes first, or needs CI/runtime validation before judgment.
- Submit review feedback, approvals, change requests, and independent code-evaluation comments only with the current ordinary GitHub user account.
- Use GitHub App or bot credentials only for author-side pull request responses, such as replying to existing review feedback, explaining pushed commits, reporting validation results, or asking for re-review when requested.
