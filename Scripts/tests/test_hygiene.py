import tempfile
import unittest
from pathlib import Path
from Scripts.validate_hygiene import documentation_errors, DOCS, POINTER
class HygieneTest(unittest.TestCase):
    def test_reports_broken_links_and_accepts_existing_and_external_targets(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for name in DOCS:
                path = root / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text('')
            (root / 'README.md').write_text('[ok](CONTRIBUTING.md) [web](https://example.com) [broken](missing.md)')
            self.assertEqual(documentation_errors(root), ['README.md: missing link target missing.md'])
    def test_rejects_raw_binary_and_malformed_pointers(self):
        self.assertIsNone(POINTER.fullmatch(b'raw binary'))
        self.assertIsNone(POINTER.fullmatch(b'version https://git-lfs.github.com/spec/v1\noid sha256:bad\nsize 3\n'))
        valid = b'version https://git-lfs.github.com/spec/v1\noid sha256:' + b'a' * 64 + b'\nsize 3\n'
        self.assertIsNotNone(POINTER.fullmatch(valid))
