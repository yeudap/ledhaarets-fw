r"""
count_prefs.py — reads / writes of every user preference, per unit. Read-only.

Implements the counting rule of docs/architecture/preferences.md §1:
  * every mention of `userPreferences.<name>` in code, after removing comments;
  * write = assignment (`=`, `+=`, ..., `++`, `--`) or a call to `.setValue(` / `.reset(`;
  * read  = any other mention;
  * unit  = file name without extension (`Shita.cpp` and `Shita.h` -> `Shita`).

The split between consumers and menu editors, and `Logic (temp)`, is done by hand from this
output (see preferences.md §1).

Validation: on JC6416 at commit dcd2cdb (7.07) every count in the 73 rows of preferences.md §4
matches this output (RegsScreen and CoreService are left out of the table by rule).

Usage (Python 3.8+, no packages needed):

    python -I tools/count_prefs.py <src_dir>            # one line per preference
    python -I tools/count_prefs.py <src_dir> --json     # machine-readable

Example — what changed between an earlier commit and the working tree, for JC6416
(Git Bash, from the repo root; <dir> is any empty folder outside the repo):

    git archive 9c75812 JC6416/src | tar -x -C <dir>
    python -I tools/count_prefs.py <dir>/JC6416/src > old.txt
    python -I tools/count_prefs.py JC6416/src > new.txt
    diff old.txt new.txt          (or compare the two files in the editor)

Output line format:   <pref>: <unit> r<reads>/w<writes>, ...

Only reads files. Writes nothing.
"""
import os
import re
import sys
import json
from collections import defaultdict

REF = re.compile(r'userPreferences\s*\.\s*(\w+)')
WRITE_AFTER = re.compile(r'\s*(?:[-+*/%&|^]|<<|>>)?=(?!=)|\s*\.\s*(?:setValue|reset)\s*\(|\s*(?:\+\+|--)')
WRITE_BEFORE = re.compile(r'(?:\+\+|--)\s*$')


def strip_comments(text):
    """Remove // and /* */ comments; keep string literals and line structure."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if c in '"\'':
            q = c
            j = i + 1
            while j < n and text[j] != q:
                j += 2 if text[j] == '\\' else 1
            out.append(text[i:j + 1])
            i = j + 1
        elif text.startswith('//', i):
            j = text.find('\n', i)
            i = n if j < 0 else j
        elif text.startswith('/*', i):
            j = text.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append('\n' * text.count('\n', i, j))
            i = j
        else:
            out.append(c)
            i += 1
    return ''.join(out)


def count(src):
    res = defaultdict(lambda: defaultdict(lambda: [0, 0]))  # pref -> unit -> [reads, writes]
    for root, _, files in os.walk(src):
        for f in files:
            if not f.endswith(('.cpp', '.h')):
                continue
            unit = os.path.splitext(f)[0]
            with open(os.path.join(root, f), encoding='utf-8-sig', errors='replace') as fh:
                text = strip_comments(fh.read())
            for m in REF.finditer(text):
                after = text[m.end():m.end() + 40]
                before = text[max(0, m.start() - 4):m.start()]
                w = bool(WRITE_AFTER.match(after)) or bool(WRITE_BEFORE.search(before))
                res[m.group(1)][unit][1 if w else 0] += 1
    return res


def main(argv):
    if len(argv) < 2 or argv[1] in ('-h', '--help'):
        print(__doc__)
        return 0
    r = count(argv[1])
    if '--json' in argv:
        print(json.dumps({p: {u: v for u, v in us.items()} for p, us in r.items()},
                         indent=1, sort_keys=True))
        return 0
    for p in sorted(r):
        print(p + ': ' + ', '.join('%s r%d/w%d' % (u, v[0], v[1]) for u, v in sorted(r[p].items())))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
