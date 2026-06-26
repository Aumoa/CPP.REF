---
name: github-app-credential-policy
description: Choose credentials for GitHub commit-adjacent and online operations in CPP.REF, especially push, branch publication, pull request creation or updates, author-side pull request responses, remote validation, and actor-sensitive review workflows.
---

# GitHub App Credential Policy

## Trigger

Use this skill before choosing credentials for GitHub operations, including push, remote branch publication, pull request creation or updates, author-side pull request responses, remote validation, or other online state changes.

Local `git commit` does not require GitHub credentials. Apply this policy when a commit is about to be pushed, associated with a pull request, or otherwise used in a remote GitHub workflow.

## Branch Classes

- Protected shared targets include `dev`, `master`, `main`, release branches, production branches, and any branch or environment used by other people.
- Isolated work branches include `codex/*`, `feature/*`, `claude/*`, `copilot/*`, and other clearly task-specific branches created for separate feature development.

## Credential Selection

- For pull request comments and review-adjacent writes, apply `Pull Request Actor Rules` before branch-class defaults.
- For protected shared targets, require ordinary user credentials and final user approval before online state changes.
- Do not use the GitHub App to push directly to protected shared targets or to bypass branch protection, review, CI, release, or production safeguards.
- For isolated work branches, prefer the GitHub App when it is available and can satisfy the needed operation.
- If the GitHub App is unavailable for an isolated work branch, ordinary user credentials may be used only when the user provides or approves them.
- If neither the GitHub App nor approved ordinary credentials are available, do not perform the online operation. Continue with local validation and report the blocker.

## Pull Request Actor Rules

- Pull request review feedback, approvals, change requests, and independent code evaluation must use the current ordinary GitHub user account.
- Do not use the GitHub App, bot identity, installation token, or connector integration credential for reviewer judgment. If suitable current-user credentials are unavailable, return the review findings to the user instead of posting them remotely.
- Pull request author or implementer responses should use the GitHub App or bot account. This includes replies to review feedback, pushed-commit explanations, pull request description updates, validation-result updates, and user-requested re-review notes.
- If an author-side response cannot be posted with the bot account, do not fall back to the ordinary user account. Stop and report the missing broker purpose, App permission, or connector capability.
- Do not assume a GitHub connector integration credential is equivalent to the local `codex-worker-aumoa` GitHub App broker credential.
- For GitHub writes that must be bot-authored, prefer the broker token and verify the resulting actor is the expected bot login, `codex-worker-aumoa[bot]`, when practical.
- If a write has a required actor and receives a 401, 403, or permission error, do not retry with a different actor. Diagnose and report the credential or permission mismatch.

## GitHub App Broker

- Prefer the local broker/client flow for GitHub App credentials.
- Broker URL: `http://127.0.0.1:5657`
- Repository: `Aumoa/CPP.REF`
- Resolve the broker secret from the current user's home directory, such as `$env:USERPROFILE\.secrets\github-auth\codex-worker-aumoa-broker.secret` in PowerShell or `~/.secrets/github-auth/codex-worker-aumoa-broker.secret` in Unix-like shells.
- Do not hard-code machine-specific absolute host paths in policy text, commands, scripts, or examples. Use shell-appropriate home directory syntax, `CODEX_WORKER_GITHUB_AUTH_SECRET`, or an explicit user-provided secret file.
- Request purpose-specific tokens instead of broad credentials.
- Pass the branch name when requesting branch-scoped tokens, and expect non-allowed branches to fail closed.
- Never print, persist, commit, or place installation tokens or broker secrets in git remotes, repository files, shell history snippets, logs, or pull request text.

## Broker Purposes

| Purpose | Branch required | Branch prefix | Use |
| --- | --- | --- | --- |
| `push-codex-branch` | yes | `codex/` | Push GitHub App-authenticated isolated work branches. |
| `create-pr` | yes | `codex/` | Create or update pull requests from isolated work branches. |
| `comment-pr` | no | n/a | Post bot-authored author-side pull request comments or responses. |

`comment-pr` is expected to use `pull_requests: write`. If ordinary issue comments are needed, confirm the App has a suitable issue-comment permission before attempting that workflow.

## App Commit Identity

- For new commits on isolated work branches that will be pushed, associated with a pull request, or otherwise used in a GitHub App-authenticated or bot-authored remote workflow, require the broker-provided App identity for the commit author and committer by default.
- If the user explicitly directs using an ordinary user account for a specific commit or workflow, that user instruction overrides the default App identity requirement for that scope.
- Resolve identity through the broker or client flow, and use the returned `gitUserName` and `gitUserEmail` with per-command git config.
- Do not rewrite existing commits solely to change author identity unless the user asks for that rewrite.
- If the broker identity endpoint is unavailable, stop before creating the commit and report the blocker unless the user explicitly authorizes using the normal local git identity for that commit.
- Do not silently fall back to an ordinary user identity, and do not invent a bot email.

## Approval Rules

- This credential policy does not replace the repository's shared-state approval rules.
- Protected shared targets still require final user approval before any online state change.
- Even on isolated work branches, avoid destructive remote operations or changes that can affect other users without explicit approval.
