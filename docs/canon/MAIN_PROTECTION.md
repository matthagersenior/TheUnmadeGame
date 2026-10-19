# Protect main — The Unmade's required canon quality gate

**Repository:** [matthagersenior/TheUnmadeGame](https://github.com/matthagersenior/TheUnmadeGame)  
**Target:** only `refs/heads/main`  
**Committed policy:** [main-canon-protection.json](../../.github/rulesets/main-canon-protection.json)

The existing `Static checks / source-checks` GitHub Actions job verifies the source-level canon ledger, ability registry, new content admissions, test suites and project contracts. The job already succeeds on `main`; **GitHub does not automatically make a check required just because CI reports it**. A GitHub **repository ruleset** is required.

## Exact protections to activate

| Setting | Required value | Reason |
| --- | --- | --- |
| Status | **Active**, branch target `main` | Prevent untested updates before they land |
| Pull request before merge | Yes | All work follows reviewable PRs, including owner work |
| Required status checks | Exactly `source-checks` | This job contains the codex coverage and change admission gates |
| Require up-to-date branch | Yes (strict) | A green branch must be tested against current `main` |
| Merge methods | Squash | One clean commit to `main`, no divergent merge histories |
| Approving reviews | 0 | Solo developer is not blocked by non-existent second reviewer |
| Bypass actors | **None**, including administrators and apps | No silent direct-push exceptions |
| Deletion / force pushes | Both prohibited | Protect permanent canon and source history |

This ruleset applies only to the main branch. It does not prevent creating a feature branch or a draft PR. Owners with suitable administration rights can still **edit the ruleset itself** through repository Settings, which is the recovery route if a validator truly breaks; changes to the ruleset are auditable.

## Installation using your own authorized GitHub login

From a workstation with Python 3 and GitHub CLI installed:

```bash
gh auth login --web
git clone https://github.com/matthagersenior/TheUnmadeGame.git
cd TheUnmadeGame
python Scripts/protect_main_ruleset.py --validate
python Scripts/protect_main_ruleset.py --apply
python Scripts/protect_main_ruleset.py --status
```

- `--validate` checks the committed rule set against the actual workflow **without any GitHub permission**.
- `--apply` uses the currently logged-in account's GitHub Administration **write** permission to create the named ruleset, or repair that specific ruleset if it exists. No token is copied into files; this does not edit other rulesets.
- `--status` performs a real API read and **fails** unless the named rule is active, strict, with no bypass, and requires the `source-checks` status.
- Run the script again to verify or reconcile future changes; it is designed to be idempotent.

If your current GitHub CLI account lacks repository administration, sign into an authorized administrator account. Do **not** post a personal access token, recovery code, or secret in chat.

## Alternative: configure directly in the GitHub website

Open the repository → **Settings** → **Rules** → **Rulesets** → **New branch ruleset**. Target `main`, turn on **Require a pull request before merging**, **Require status checks to pass**, and **Require branches to be up to date**. Select the exact `source-checks` check. Block deletions and force pushes. Leave the bypass list empty; keep required approvals at zero, and save the ruleset as **Active**. Check the resulting rule matches [the committed policy](../../.github/rulesets/main-canon-protection.json).

You can review active rulesets at [repository Rules settings](https://github.com/matthagersenior/TheUnmadeGame/settings/rules) and verify the status with `--status`.

## Current administrative limitation

The connected ChatGPT GitHub installation supports code, PRs and CI, but **does not expose a ruleset creation/update operation with repository Administration write permission**. It can commit and validate this policy but cannot honestly claim to have activated it. GitHub requires this permission for the Rulesets create API. Once activated by an authorized admin, GitHub itself prevents a disallowed merge. This is stronger than the post-push CI safeguard.

## Avoid accidental check-name drift

The required context is `source-checks`, **not** `Static checks` (workflow title) and not the individual step labels. The root workflow contains `jobs: source-checks:` and runs for `pull_request` and pushes to `main`. Renaming this job without changing the active ruleset will strand PRs waiting for a check that never reports. The CI policy validation tests this relationship.

## Confirm operation

1. Verify `python Scripts/protect_main_ruleset.py --status` reports **PASS** against GitHub.
2. Open an ordinary PR and check that its `source-checks` result is required for merge.
3. A PR whose canon admission check fails must show **merge blocked**. Do not manufacture a failure directly on `main`.
4. Ensure the repository has no routine bypass actors.
5. Keep status checks green and merge all authorized updates through PRs.

**Important:** the ruleset stops merges with missing or failed checks. It does **not** prove that a fabricated but syntactically valid source claim is faithful to the lore. Continue human review and Unreal playtests for narrative and experiential truth.
