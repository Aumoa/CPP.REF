---
name: ayla-pr-review
description: Repository-local pull request review guidance for Ayla engine changes. Use when Codex reviews PRs, branch diffs, CI failures, architecture changes, class responsibilities, duplication, or merge readiness in this repository.
---

# Ayla PR Review

## Overview

Ayla PR reviews should protect correctness, build health, and the repository's intended architecture without turning style preference into noise.

## Review Priorities

- Lead with actionable findings ordered by severity.
- Ground findings in concrete source locations, logs, commands, or diffs.
- Separate blockers from residual risks and optional follow-up ideas.
- When reviewing CI or build changes, connect workflow edits back to the actual AylaBuildTool, module rules, generated files, and platform-specific code paths they exercise.

## GitHub Actions

- Use `gh` to inspect Actions runs, jobs, and logs when the command is available.
- When `gh` is available, verify compile results for each supported platform in GitHub Actions: Windows, macOS, and Linux. Prefer job-level conclusions over only checking the overall workflow conclusion.
- If `gh` or Actions access is unavailable, state that Actions validation was skipped and perform a stricter source and workflow review for Windows, macOS, and Linux support.
- Local-only review actions, including read-only checks, local commits, and local merges, may be performed at Codex's discretion.
- Do not perform actions that can affect other users, remote branches, hosted services, or shared state unless the user explicitly approves that specific action first.
- If Actions validation requires a push or another shared-state action, report what will be done and wait for the user's decision before proceeding.

## Class Responsibility

- Check whether each meaningful feature is owned by a clear class or collaborator.
- Flag classes that combine unrelated responsibilities when that coupling makes behavior harder to extend, test, or review.
- Prefer changes that move behavior into cohesive feature units rather than scattered special cases.
- Do not request extra splitting when the existing responsibility boundary is already understandable and further separation would mostly add indirection.
- Do not ask authors to undo harmless fine-grained separation solely because it is more detailed than necessary.

## Duplication

- Flag duplicated code when it represents the same feature, policy, decision, or platform behavior and merging it would reduce bug risk or future maintenance.
- Prefer one cohesive implementation for repeated behavior that must evolve together.
- Avoid demanding abstractions for small incidental duplication when the abstraction would be noisier than the repeated code.

## Review Comments

- Write external PR review comments in English unless the user asks otherwise.
- Explain the interpretation and recommendation to the user in Korean when the surrounding conversation is Korean.
- Clearly state whether the reviewed change is safe to merge, needs fixes first, or needs CI/runtime validation before judgment.
