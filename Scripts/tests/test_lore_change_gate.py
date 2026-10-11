import unittest
from Scripts.check_lore_change import check_changes, CANON, LEDGER, RELEASE, MANIFEST

class LoreUpdateGateTest(unittest.TestCase):
    def test_gameplay_change_requires_canon_ledger_and_release_receipts(self):
        changed=["Source/TheUnmadeGame/Public/World/NewQuest.h"]
        self.assertEqual(check_changes(changed),[CANON,LEDGER,RELEASE,MANIFEST])
        self.assertEqual(check_changes(changed+[CANON]),[LEDGER,RELEASE,MANIFEST])
        self.assertEqual(check_changes(changed+[CANON,LEDGER]),[RELEASE,MANIFEST])
        self.assertEqual(check_changes(changed+[CANON,LEDGER,RELEASE]),[MANIFEST])
        self.assertEqual(check_changes(changed+[CANON,LEDGER,RELEASE,MANIFEST]),[])
    def test_docs_and_test_only_changes_need_no_recursion(self):
        self.assertEqual(check_changes(["README.md","Scripts/tests/test_lore_continuity.py"]),[])
    def test_non_unreal_changes_are_not_falsely_triggered(self):
        self.assertEqual(check_changes([".github/workflows/static-checks.yml"]),[])
if __name__=="__main__":unittest.main()
