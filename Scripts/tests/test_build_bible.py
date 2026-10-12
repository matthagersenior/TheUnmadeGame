import json
import tempfile
import unittest
from pathlib import Path
from Scripts.build_bible import build

class CanonicalManuscriptTest(unittest.TestCase):
    def fixture(self, root, sources):
        lore = root / 'docs/lore'
        lore.mkdir(parents=True)
        (lore / 'bible_sources.json').write_text(json.dumps({'schema': 1, 'title': 'Canon', 'scope': 'Test', 'sources': sources}))
        return lore

    def test_preserves_source_order_and_prose_and_rebuilds_identically(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            lore = self.fixture(root, ['docs/lore/b.md', 'docs/lore/a.md'])
            (lore / 'a.md').write_text('# A\n\nThe remembered road.')
            (lore / 'b.md').write_text('# B\n\nA choice remains open.')
            text, hashes = build(root)
            self.assertLess(text.index('# B'), text.index('# A'))
            self.assertIn('The remembered road.', text)
            self.assertEqual((text, hashes), build(root))
            (lore / 'a.md').write_text('# A\n\nChanged evidence.')
            changed, new_hashes = build(root)
            self.assertNotEqual(text, changed)
            self.assertNotEqual(hashes['manuscript_sha256'], new_hashes['manuscript_sha256'])

    def test_rejects_source_outside_canon(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.fixture(root, ['README.md'])
            (root / 'README.md').write_text('Not canon')
            with self.assertRaises(ValueError):
                build(root)

    def test_missing_source_fails_instead_of_publishing_partial_book(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.fixture(root, ['docs/lore/missing.md'])
            with self.assertRaises(FileNotFoundError):
                build(root)
