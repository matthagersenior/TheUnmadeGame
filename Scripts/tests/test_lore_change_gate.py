import unittest
from Scripts.check_lore_change import check_changes, CANON, LEDGER

class LoreUpdateGateTest(unittest.TestCase):
    def test_gameplay_change_requires_canon_and_ledger_only(self):
        changed=["Source/TheUnmadeGame/Public/World/NewQuest.h"]
        self.assertEqual(check_changes(changed),[CANON,LEDGER])
        self.assertEqual(check_changes(changed+[CANON]),[LEDGER])
        self.assertEqual(check_changes(changed+[CANON,LEDGER]),[])
    def test_optional_illustration_receipts_do_not_replace_canon_updates(self):
        receipts=['docs/lore/LATEST_ILLUSTRATED_RELEASE.md',
                  'docs/lore/RELEASE_SHA256_MANIFEST.json']
        self.assertEqual(check_changes(receipts),[])
        self.assertEqual(check_changes(['Source/TheUnmadeGame/Public/World/NewQuest.h']+receipts),
                         [CANON,LEDGER])
    def test_docs_and_test_only_changes_need_no_recursion(self):
        self.assertEqual(check_changes(["README.md","Scripts/tests/test_lore_continuity.py"]),[])
    def test_non_unreal_changes_are_not_falsely_triggered(self):
        self.assertEqual(check_changes([".github/workflows/static-checks.yml"]),[])
if __name__=="__main__":unittest.main()
