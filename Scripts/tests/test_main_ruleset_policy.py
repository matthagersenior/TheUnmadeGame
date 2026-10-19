"""Guard against relaxing the protection for the canon-gated main branch."""
import copy
import json
from pathlib import Path
import unittest
from unittest.mock import patch

from Scripts.protect_main_ruleset import (
    POLICY, WORKFLOW, validate_config, live_compliance, main,
)


class MainRulesetPolicyTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.policy = json.loads(POLICY.read_text(encoding="utf-8"))
        cls.workflow = WORKFLOW.read_text(encoding="utf-8")

    def test_committed_policy_targets_real_checks(self):
        self.assertEqual([], validate_config(self.policy, self.workflow))
        self.assertEqual(0, main(["--validate"]))

    def test_bypass_exemptions_are_rejected(self):
        changed = copy.deepcopy(self.policy)
        changed["bypass_actors"] = [{
            "actor_id": 5, "actor_type": "RepositoryRole", "bypass_mode": "always"
        }]
        self.assertIn("Bypass actors must be empty, including administrators",
                      validate_config(changed, self.workflow))

    def test_source_checks_must_be_strict(self):
        changed = copy.deepcopy(self.policy)
        rule = next(r for r in changed["rules"] if r["type"] == "required_status_checks")
        rule["parameters"]["strict_required_status_checks_policy"] = False
        self.assertTrue(validate_config(changed, self.workflow))

    def test_status_check_cannot_be_replaced_by_fake_one(self):
        changed = copy.deepcopy(self.policy)
        rule = next(r for r in changed["rules"] if r["type"] == "required_status_checks")
        rule["parameters"]["required_status_checks"] = [{"context": "ignored"}]
        self.assertTrue(validate_config(changed, self.workflow))

    def test_no_premature_approval_requirements_on_solo_project(self):
        changed = copy.deepcopy(self.policy)
        rule = next(r for r in changed["rules"] if r["type"] == "pull_request")
        rule["parameters"]["required_approving_review_count"] = 1
        self.assertTrue(validate_config(changed, self.workflow))

    def test_prohibit_direct_main_pushes_and_force_pushes(self):
        changed = copy.deepcopy(self.policy)
        changed["rules"] = [
            r for r in changed["rules"] if r["type"] not in ("pull_request", "non_fast_forward")
        ]
        self.assertTrue(validate_config(changed, self.workflow))

    def test_wrong_target_is_rejected(self):
        changed = copy.deepcopy(self.policy)
        changed["conditions"]["ref_name"]["include"] = ["refs/heads/development"]
        self.assertTrue(validate_config(changed, self.workflow))

    def test_workflow_must_retain_codex_verification(self):
        for marker in ("Scripts/check_canon_change.py --base",
                       "Scripts/validate_canon_encyclopedia.py"):
            self.assertTrue(validate_config(self.policy, self.workflow.replace(marker, "DISABLED")))

    def test_live_policy_checks_detection_of_missing_required_rule(self):
        changed = copy.deepcopy(self.policy)
        changed["rules"] = [r for r in changed["rules"]
                            if r["type"] != "required_status_checks"]
        self.assertTrue(any("Missing live rule" in x
                            for x in live_compliance(changed, self.policy)))

    def test_live_ruleset_must_have_no_admin_bypass(self):
        changed = copy.deepcopy(self.policy)
        changed["bypass_actors"] = [{
            "actor_type": "RepositoryRole", "actor_id": 5, "bypass_mode": "always"
        }]
        self.assertTrue(any("bypass" in e
                            for e in live_compliance(changed, self.policy)))

    def test_no_ruleset_is_reported_inactive(self):
        with patch("Scripts.protect_main_ruleset.locate_ruleset", return_value=None):
            self.assertEqual(1, main(["--status"]))


if __name__ == "__main__":
    unittest.main()
