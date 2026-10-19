#!/usr/bin/env python3
"""Audit or install The Unmade's main ruleset with the repository owner's GitHub CLI.

--validate is fully offline and runs in CI. --status and --apply require a
GitHub authentication with Administration:write on this particular repository.
The connected GitHub App used by ChatGPT cannot perform administration.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
POLICY = ROOT / ".github/rulesets/main-canon-protection.json"
WORKFLOW = ROOT / ".github/workflows/static-checks.yml"
REPO = "matthagersenior/TheUnmadeGame"
API = f"repos/{REPO}/rulesets"
CHECK = "source-checks"


def validate_config(policy: dict, workflow: str) -> list[str]:
    errors = []
    if policy.get("name") != "The Unmade — Canon-Gated Main":
        errors.append("Ruleset name drift")
    if policy.get("target") != "branch" or policy.get("enforcement") != "active":
        errors.append("Protection must be an active branch ruleset")
    if policy.get("bypass_actors") != []:
        errors.append("Bypass actors must be empty, including administrators")
    if policy.get("conditions") != {"ref_name": {"include": ["refs/heads/main"], "exclude": []}}:
        errors.append("Ruleset must apply exactly to refs/heads/main")
    rules = policy.get("rules")
    if not isinstance(rules, list):
        return errors + ["Ruleset must have a rules array"]
    types = [r.get("type") for r in rules if isinstance(r, dict)]
    if len(types) != len(rules) or len(set(types)) != len(types):
        errors.append("Duplicate or malformed rules")
    required = {"pull_request", "required_status_checks", "non_fast_forward", "deletion"}
    if set(types) != required:
        errors.append(f"Required rule set is {sorted(required)}; got {sorted(str(t) for t in types)}")
    by_type = {r["type"]: r for r in rules if isinstance(r, dict) and isinstance(r.get("type"), str)}
    pr = by_type.get("pull_request", {}).get("parameters", {})
    if pr.get("required_approving_review_count") != 0:
        errors.append("Solo founder should not require unavailable reviewers")
    if pr.get("allowed_merge_methods") != ["squash"]:
        errors.append("Main should use only squash PR merges")
    if pr.get("require_code_owner_review") or pr.get("require_last_push_approval"):
        errors.append("Do not introduce implicit second-person requirements")
    check = by_type.get("required_status_checks", {}).get("parameters", {})
    if check.get("required_status_checks") != [{"context": CHECK}]:
        errors.append("Require precisely the source-checks status context")
    if check.get("strict_required_status_checks_policy") is not True:
        errors.append("Check must pass against latest main")
    if check.get("do_not_enforce_on_create") is not False:
        errors.append("No silent creation exemption")
    if "name: Static checks" not in workflow or "  source-checks:" not in workflow:
        errors.append("Required CI check does not exist in workflow")
    if "  pull_request:" not in workflow or "    branches: [main]" not in workflow:
        errors.append("Static CI must execute for PRs and pushes to main")
    if "Scripts/check_canon_change.py --base" not in workflow:
        errors.append("Existing change-impact gate must remain part of CI")
    if "Scripts/validate_canon_encyclopedia.py" not in workflow:
        errors.append("Existing full codex validator must remain part of CI")
    return errors


def gh_api(*args: str) -> object:
    try:
        result = subprocess.run(
            ["gh", "api", "-H", "Accept: application/vnd.github+json", *args],
            capture_output=True, text=True, check=False,
        )
    except FileNotFoundError as e:
        raise RuntimeError("GitHub CLI missing. Install gh and run gh auth login.") from e
    if result.returncode:
        raise RuntimeError(
            "GitHub API operation failed; check gh auth status and repository "
            "Administration:write permissions. " + result.stderr.strip()[:650]
        )
    try:
        return json.loads(result.stdout)
    except json.JSONDecodeError as e:
        raise RuntimeError("GitHub returned a non-JSON response") from e


def locate_ruleset(name: str) -> int | None:
    rules = gh_api(API + "?per_page=100")
    if not isinstance(rules, list):
        raise RuntimeError("Unexpected GitHub ruleset listing")
    matches = [
        row for row in rules if row.get("name") == name
        and row.get("source_type", "Repository") == "Repository"
    ]
    if len(matches) > 1:
        raise RuntimeError("Multiple rulesets with the same name; refusing to pick one")
    return int(matches[0]["id"]) if matches else None


def live_compliance(actual: dict, expected: dict) -> list[str]:
    # GitHub may return extra default parameters; compare the meaningful
    # security guarantees rather than assuming byte-for-byte JSON equality.
    errors = []
    for k in ("name", "target", "enforcement"):
        if actual.get(k) != expected.get(k):
            errors.append(f"live {k} does not match")
    if actual.get("bypass_actors", []) != []:
        errors.append("Live ruleset has bypass actors")
    ref = actual.get("conditions", {}).get("ref_name", {})
    if ref != expected["conditions"]["ref_name"]:
        errors.append("Live ruleset does not target only main")
    ar = {r.get("type"): r for r in actual.get("rules", [])}
    er = {r.get("type"): r for r in expected["rules"]}
    for t in er:
        if t not in ar:
            errors.append(f"Missing live rule {t}")
    if "required_status_checks" in ar:
        p = ar["required_status_checks"].get("parameters", {})
        req = p.get("required_status_checks", [])
        if [row.get("context") for row in req] != [CHECK]:
            errors.append("Live ruleset does not require exactly source-checks")
        if p.get("strict_required_status_checks_policy") is not True:
            errors.append("Live checks are not strict")
    if "pull_request" in ar:
        p = ar["pull_request"].get("parameters", {})
        if p.get("required_approving_review_count") != 0:
            errors.append("Live rule unexpectedly requires human approval")
        if p.get("allowed_merge_methods") != ["squash"]:
            errors.append("Live rule permits or blocks unexpected merge methods")
    return errors


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument("--validate", action="store_true", help="Check policy & workflow without credentials")
    action.add_argument("--status", action="store_true", help="Read current live protection using gh auth")
    action.add_argument("--apply", action="store_true", help="Create/update protection with admin gh auth")
    args = parser.parse_args(argv)
    try:
        expected = json.loads(POLICY.read_text(encoding="utf-8"))
        workflow = WORKFLOW.read_text(encoding="utf-8")
        errors = validate_config(expected, workflow)
        if errors:
            for e in errors:
                print("POLICY ERROR:", e, file=sys.stderr)
            return 1
        if args.validate:
            print("PASS: committed main ruleset is well-formed and names real CI gates; activation is separate")
            return 0
        ruleset_id = locate_ruleset(expected["name"])
        if args.apply:
            if ruleset_id is None:
                response = gh_api("-X", "POST", API, "--input", str(POLICY))
                ruleset_id = int(response["id"])
                print("Created main protection ruleset:", ruleset_id)
            else:
                live = gh_api(f"{API}/{ruleset_id}")
                if live_compliance(live, expected):
                    gh_api("-X", "PUT", f"{API}/{ruleset_id}", "--input", str(POLICY))
                    print("Updated named main protection ruleset:", ruleset_id)
        if ruleset_id is None:
            print("NOT ACTIVE: no named main ruleset exists. Run --apply with administrator auth.")
            return 1
        live = gh_api(f"{API}/{ruleset_id}")
        problems = live_compliance(live, expected)
        if problems:
            for e in problems:
                print("NOT COMPLIANT:", e, file=sys.stderr)
            return 1
        print(f"PASS: live main ruleset {ruleset_id} is active, strict, has no bypass, and requires source-checks")
        return 0
    except (OSError, ValueError, KeyError, RuntimeError) as e:
        print("RULESET ERROR:", e, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
