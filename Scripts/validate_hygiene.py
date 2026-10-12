"""Check local documentation links and tracked binary LFS pointers."""
from pathlib import Path
import re
import subprocess
ROOT = Path(__file__).resolve().parents[1]
DOCS = ('README.md', 'CONTRIBUTING.md', 'docs/CHANGELOG.md', 'docs/publishing.md',
        'docs/qa/git-lfs.md', 'docs/qa/unreal-ci.md', 'docs/lore/LATEST_ILLUSTRATED_RELEASE.md')
BINARY = {'.uasset', '.umap', '.ubulk', '.uexp', '.wav', '.fbx', '.png', '.glb',
          '.jpg', '.jpeg', '.tga', '.exr', '.ogg', '.mp3', '.mp4', '.blend'}
POINTER = re.compile(rb'version https://git-lfs.github.com/spec/v1\noid sha256:[0-9a-f]{64}\nsize [0-9]+\n')
def documentation_errors(root):
    problems = []
    for name in DOCS:
        path = root / name
        if not path.is_file():
            problems.append(f'Missing documentation: {name}')
            continue
        for target in re.findall(r'\]\(([^)\s]+)\)', path.read_text(encoding='utf-8')):
            if target.startswith(('#', 'https://', 'http://', 'mailto:')):
                continue
            local = target.split('#')[0]
            if local and not (path.parent / local).exists():
                problems.append(f'{name}: missing link target {target}')
    return problems
def main():
    problems = documentation_errors(ROOT)
    paths = subprocess.check_output(['git', 'ls-files', '-z'], cwd=ROOT).decode().split('\0')
    count = 0
    for name in paths:
        if Path(name).suffix.lower() not in BINARY:
            continue
        count += 1
        data = subprocess.check_output(['git', 'show', f':{name}'], cwd=ROOT)
        if not POINTER.fullmatch(data):
            problems.append(f'{name}: tracked binary must be a valid Git LFS pointer')
    if problems:
        raise SystemExit('\n'.join(problems))
    print(f'PASS: documentation links and {count} tracked binary LFS pointers; remote LFS transfer is separate')
if __name__ == '__main__':
    main()
