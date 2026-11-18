r"""
gen_deps_doc.py — generated blocks of docs/architecture/dependencies.md, from count_deps.py.

The map in dependencies.md is regenerated at every production sync and at the end of every
sub-stage that changes dependencies (rule in its §1). Parts of it are pure tool output; they sit
between markers and are written only by this script:

    <!-- generated:<block>:begin -->
    <!-- generated:<block>:end -->

    edges     §4 edge table (rows of `count_deps.py`, plus the "mark" column)
    includes  §9 include-only edges (`count_deps.py --includes`; quoted includes only)
    a1 .. a4  appendix A matrices (topic x topic, States files, Screens files, other units)

Everything else in the document is written by hand, with the editor tools (CLAUDE.md, working
rules). The "mark" column is computed from the level table of §2 (LEVELS below) and the two
intended edges recorded in docs/architecture/notes.md (INTENDED below). JC6416 only: the row
lists of A.2 / A.3 / A.4 follow its units, and the script stops if a unit is missing or new.

Usage (Python 3.8+, no packages needed; run from the repo root, Git Bash):

    python -I tools/gen_deps_doc.py print <block>   # print one block of the current tree
    python -I tools/gen_deps_doc.py write           # fill all marked blocks in the document
    python -I tools/gen_deps_doc.py check           # document up to date and consistent?
    python -I tools/gen_deps_doc.py report          # helper data for the hand-written sections
    python -I tools/gen_deps_doc.py selfcheck       # reproduce the previous map (9c75812)

    --src <dir>   source tree (default JC6416/src)
    --doc <file>  document (default docs/architecture/dependencies.md)

write   replaces only the lines between each pair of markers, with a blank line before and after
        the block, and writes the file back with CRLF. It refuses a document that is not pure
        CRLF or that lacks a marker.
check   exit code 1 if a block differs from what `print` would produce, if the §4 total line
        does not match, or if the §3 graph / threshold table or the A.2 total row disagree with A.1.
selfcheck
        `git archive 9c75812 JC6416/src` into a temporary folder, generates the blocks from it and
        compares them with the document at e7f8efb (the map before the regeneration). Expected
        differences, and nothing else: §4 examples in tie order (the tool sorts ties by name) and
        the identifier count of two edges (MainStateInfoSets->DayInfo, MainState->DayInfo); A.1
        `Prefs -> States` `inc` (angle-bracket include, not seen by the tool); A.4 unit names
        inside parentheses (uniform rule here) and the `UserPreferences` include. §9 was a hand
        list then and is not compared.

Writes nothing except the document in `write` mode. No bytecode is written.
"""
import importlib.util
import io
import os
import re
import subprocess
import sys
import tarfile
import tempfile
from collections import defaultdict, OrderedDict

sys.dont_write_bytecode = True
sys.stdout.reconfigure(encoding='utf-8', newline='\n')

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
_spec = importlib.util.spec_from_file_location('count_deps', os.path.join(HERE, 'count_deps.py'))
CD = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(CD)

SELFCHECK_TREE = '9c75812'
SELFCHECK_DOC = 'e7f8efb'

# ------------------------------------------------------------------ topics and levels (§2, §3)

TOPICS = ['main', 'BLE', 'BTN', 'Logic', 'States', 'Screens', 'Render', 'DB', 'Prefs', 'CLK', 'HW', 'Meta']
TOPIC_LABEL = {'Meta': 'Metadata'}


def topic(u):
    if u == 'main':
        return 'main'
    if u in ('Area', 'Text', 'Colors', 'Fonts'):
        return 'Render'
    if u in ('Display', 'Hardware'):
        return 'HW'
    if u == 'Metadata':
        return 'Meta'
    if u == 'Logic':
        return 'Logic'
    if u == 'DataBase':
        return 'DB'
    return {'BLE': 'BLE', 'BTN': 'BTN', 'States': 'States', 'Screens': 'Screens', 'DB': 'DB',
            'Preferences': 'Prefs', 'CLK': 'CLK'}[u.split('/')[0]]


DISPLAY_UNITS = {'Area', 'Text', 'Colors', 'Fonts', 'States/Digit', 'States/UpdateState', 'DB/Strings'}
HW_UNITS = {'Display', 'Hardware', 'BTN/ButtonDriver', 'BTN/ButtonLH1022', 'BTN/ButtonLH0826',
            'CLK/ClockDriver', 'CLK/ClockDS3231', 'CLK/ClockRV3028'}


def level(u):
    """(number, sublayer) per §2; None for Metadata (configuration, anyone may use it)."""
    if u == 'Metadata':
        return None
    if u == 'main':
        return (0, '')
    if u in HW_UNITS:
        return (5, '')
    if u.startswith('BLE/') or u == 'BTN/Button':
        return (1, '')
    if u.startswith('Screens/') or u in DISPLAY_UNITS:
        return (3, 'display')
    if u.startswith('DB/') or u == 'DataBase':
        return (3, 'domain')
    if u == 'Logic' or u.startswith('States/'):
        return (2, '')
    if u.startswith('Preferences/') or u == 'CLK/Clock':
        return (4, '')
    raise ValueError('unit without a level: %s' % u)


INTENDED = {('BTN/Button', 'BLE/Commands'): 'מכוון: לחיצה מדומה',
            ('BLE/OtaProtocol', 'States/UpdateState'): 'מכוון: מסך התקדמות OTA'}


def mark(a, b):
    if (a, b) in INTENDED:
        return INTENDED[(a, b)]
    la, lb = level(a), level(b)
    if la is None or lb is None:
        return ''
    if lb[0] < la[0]:
        return 'נגד הכיוון'
    if la[0] == lb[0] == 3 and la[1] != lb[1]:
        return 'צדדית'
    return ''


# ------------------------------------------------------------------ the model of one tree

class Tree:
    def __init__(self, src):
        self.files, _, self.edges = CD.analyze(src)
        self.units = sorted(set(u for _, u, _ in self.files))
        self.inc = CD.count_includes(self.files)
        self.rows = CD.summarize(self.edges)

    def topic_matrix(self):
        mat = defaultdict(int)
        for (a, b), labels in self.edges.items():
            if topic(a) != topic(b):
                mat[(topic(a), topic(b))] += sum(labels.values())
        inc = set()
        for (a, b) in self.inc:
            ta, tb = topic(a), topic(b)
            if ta != tb and not mat.get((ta, tb)):
                inc.add((ta, tb))
        return mat, inc


def cells(row):
    return '| ' + ' | '.join(row) + ' |'


def block_edges(t):
    out = ['| # | מ- | אל | הפניות | מזהים | דוגמאות | סימון |',
           '| --- | --- | --- | --- | --- | --- | --- |']
    for i, (a, b, n, ids, top) in enumerate(t.rows, 1):
        out.append('| %d | `%s` | `%s` | %d | %d | %s | %s |' % (i, a, b, n, ids, CD.fmt_top(top), mark(a, b)))
    return out


def block_includes(t):
    return ['- `%s` → `%s` (%s)' % (a, b, ', '.join('`%s`' % s for s in t.inc[(a, b)]))
            for a, b in sorted(k for k in t.inc if k not in t.edges)]


def block_a1(t):
    mat, inc = t.topic_matrix()
    out = ['| מ-\\אל | ' + ' | '.join(TOPICS) + ' |', '| --- |' + ' --- |' * len(TOPICS)]
    for ta in TOPICS:
        row = ['**%s**' % ta]
        for tb in TOPICS:
            if ta == tb:
                row.append('·')
            elif mat.get((ta, tb)):
                row.append(str(mat[(ta, tb)]))
            else:
                row.append('inc' if (ta, tb) in inc else '')
        out.append(cells(row))
    return out


A2_ROWS = ['States/States.h', 'States/State', 'States/States', 'States/MainState', 'States/MainStateInfoSets',
           'States/MainStateTimeSets', 'States/Digit', 'States/UpdateState', 'States/ColorState',
           'States/BrightnessState', 'States/FontState', 'States/EdotState', 'States/MinhagState',
           'States/ShitaState', 'States/OfekState', 'States/SetClockState', 'States/ConfigState',
           'States/SetInputText', 'States/SetTochenInfoState', 'States/SetTochenTimeState']
A2_COLS = ['BLE', 'Logic', 'intra', 'Screens', 'Render', 'DB', 'Prefs', 'CLK', 'HW']


def block_a2(t):
    states = set(u for u in t.units if topic(u) == 'States')
    if states != set(A2_ROWS):
        raise SystemExit('A.2: States units changed: %s' % sorted(states ^ set(A2_ROWS)))
    out = ['| States/ | BLE | Logic | intra | Screens | Render | DB (מתוכם Strings) | Prefs | CLK | HW |',
           '| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |']
    tot = defaultdict(int)
    for u in A2_ROWS:
        c = defaultdict(int)
        for (a, b), labels in t.edges.items():
            if a != u:
                continue
            n = sum(labels.values())
            key = 'intra' if topic(b) == 'States' else topic(b)
            if key not in A2_COLS:
                raise SystemExit('A.2: no column for %s -> %s' % (a, b))
            c[key] += n
            if b == 'DB/Strings':
                c['Strings'] += n
        name = {'States/States.h': 'States.h', 'States/States': 'States.cpp'}.get(u, u.split('/')[-1])
        row = [name]
        for k in A2_COLS:
            if k == 'DB':
                row.append('%d (%d)' % (c['DB'], c['Strings']) if c['DB'] else '')
            else:
                row.append(str(c[k]) if c[k] else '')
            if k != 'intra':
                tot[k] += c[k]
        tot['Strings'] += c['Strings']
        out.append(cells(row))
    row = ['**סה"כ (בלי intra)**']
    for k in A2_COLS:
        if k == 'intra':
            row.append('')
        elif k == 'DB':
            row.append('%d (%d)' % (tot['DB'], tot['Strings']))
        else:
            row.append(str(tot[k]) if tot[k] else '')
    out.append(cells(row))
    return out


A3_ROWS = ['Screens/Screens.h', 'Screens/MainScreen', 'Screens/RegsScreen', 'Screens/SetClockScreen',
           'Screens/MenuScreen', 'Screens/messageScreen', 'Screens/VersionScreen', 'Screens/TextScreen',
           'Screens/AboutScreen', 'Screens/ResetScreen', 'Screens/ColorScreen', 'Screens/BrightnessScreen',
           'Screens/LogoScreen']
A3_COLS = ['Screens.h', 'Area', 'Colors', 'Text', 'Fonts', 'Prefs', 'DB', 'Display', 'Metadata']


def a3_col(a, b):
    if b == 'Screens/Screens.h':
        return 'Screens.h'
    if b in ('Area', 'Colors', 'Text', 'Fonts', 'Display', 'Metadata'):
        return b
    if topic(b) in ('Prefs', 'DB'):
        return topic(b)
    raise SystemExit('A.3: no column for %s -> %s' % (a, b))


def block_a3(t):
    screens = set(u for u in t.units if topic(u) == 'Screens')
    if screens != set(A3_ROWS):
        raise SystemExit('A.3: Screens units changed: %s' % sorted(screens ^ set(A3_ROWS)))
    out = ['| Screens/ | ' + ' | '.join(A3_COLS) + ' |', '| --- |' + ' --- |' * len(A3_COLS)]
    for u in A3_ROWS:
        c = defaultdict(int)
        for (a, b), labels in t.edges.items():
            if a == u:
                c[a3_col(a, b)] += sum(labels.values())
        row = [u.split('/')[-1]]
        for k in A3_COLS:
            row.append('·' if (u == 'Screens/Screens.h' and k == 'Screens.h') else (str(c[k]) if c[k] else ''))
        out.append(cells(row))
    return out


A4_GROUPS = [['main'], ['Logic'], ['Hardware'], ['Display'], ['Area'], ['Text'], ['Colors', 'Fonts', 'Metadata'],
             ['BLE/Bluetooth'], ['BLE/CoreService'], ['BLE/Authmanager'], ['BLE/Commands'],
             ['BLE/TextCommandHandler'], ['BLE/FirmwareService'], ['BLE/OtaProtocol'],
             ['BTN/Button'], ['BTN/ButtonDriver'], ['BTN/ButtonLH1022'], ['BTN/ButtonLH0826'],
             ['CLK/Clock'], ['CLK/ClockDriver'], ['CLK/ClockDS3231', 'CLK/ClockRV3028'],
             ['DB/DayInfo'], ['DB/Shita'], ['DB/Place'], ['DB/DB'], ['DB/Sun'], ['DB/Zman'], ['DB/HebrewDate'],
             ['DB/DafYomi'], ['DB/Strings', 'DB/Dict', 'DB/LoaziDate', 'DataBase'],
             ['Preferences/UserPreferences'],
             ['Preferences/SystemPrefs'], ['Preferences/UserContent'], ['Preferences/PresentationPrefs'],
             ['Preferences/DisplayPrefs'], ['Preferences/LogicPrefs'], ['Preferences/DataPrefs'],
             ['Preferences/PrefsViews']]
OPTIONAL_UNITS = {'Preferences/SystemPrefs', 'Preferences/UserContent', 'Preferences/PresentationPrefs',
                  'Preferences/DisplayPrefs', 'Preferences/LogicPrefs', 'Preferences/DataPrefs',
                  'Preferences/PrefsViews'}   # absent before stage 2a


def a4_text(t, u):
    ext, intra = defaultdict(int), defaultdict(int)
    for (a, b), labels in t.edges.items():
        if a != u:
            continue
        n = sum(labels.values())
        if topic(b) == topic(u):
            intra[b] += n
        else:
            ext[topic(b)] += n
    parts = ['%s %d' % (TOPIC_LABEL.get(x, x), ext[x]) for x in sorted(ext, key=lambda x: (-ext[x], TOPICS.index(x)))]
    incs = sorted(set(topic(b) for (a, b) in t.inc if a == u and topic(b) != topic(u) and not ext.get(topic(b))),
                  key=TOPICS.index)
    parts += ['(include %s)' % TOPIC_LABEL.get(x, x) for x in incs]
    if intra:
        n = sum(intra.values())
        if len(intra) == 1:
            parts.append('(%s: %s %d)' % (topic(u), list(intra)[0].split('/')[-1], n))
        else:
            parts.append('(%s %d)' % (topic(u), n))
    return ', '.join(parts) if parts else '—'


def block_a4(t):
    present = set(t.units)
    groups = []
    for g in A4_GROUPS:
        missing = [u for u in g if u not in present]
        if missing and not set(missing) <= OPTIONAL_UNITS:
            raise SystemExit('A.4: unit missing: %s' % missing)
        if not missing:
            groups.append(g)
    listed = set(A2_ROWS) | set(A3_ROWS) | set(u for g in groups for u in g)
    extra = [u for u in t.units if u not in listed]
    if extra:
        raise SystemExit('A.4: unit not listed: %s' % extra)
    out = ['| יחידה | יעדים |', '| --- | --- |']
    for g in groups:
        texts = [a4_text(t, u) for u in g]
        name = lambda u: 'Hardware.h' if u == 'Hardware' else u
        if len(set(texts)) == 1:
            x = texts[0]
            if len(g) > 1 and x != '—':
                x = x[:-1] + ' כל אחת)' if x.endswith(')') else x + ' (כל אחת)'
            out.append('| %s | %s |' % (', '.join(name(u) for u in g), x))
        else:
            out += ['| %s | %s |' % (name(u), x) for u, x in zip(g, texts)]
    return out


BLOCKS = OrderedDict([('edges', block_edges), ('includes', block_includes), ('a1', block_a1),
                      ('a2', block_a2), ('a3', block_a3), ('a4', block_a4)])

# ------------------------------------------------------------------ document helpers


def read_doc(path):
    raw = open(path, 'rb').read().decode('utf-8')
    crlf = raw.count('\r\n')
    if crlf == 0 or raw.count('\n') != crlf or raw.count('\r') != crlf:
        raise SystemExit('%s is not pure CRLF' % path)
    return raw.split('\r\n')


def markers(lines, name):
    b, e = '<!-- generated:%s:begin -->' % name, '<!-- generated:%s:end -->' % name
    if lines.count(b) != 1 or lines.count(e) != 1:
        raise SystemExit('marker for block "%s" missing or duplicated' % name)
    i, j = lines.index(b), lines.index(e)
    if i >= j:
        raise SystemExit('markers of block "%s" out of order' % name)
    return i, j


def doc_block(lines, name):
    i, j = markers(lines, name)
    return [x for x in lines[i + 1:j] if x]


def write(t, path):
    lines = read_doc(path)
    for name, fn in BLOCKS.items():
        i, j = markers(lines, name)
        lines = lines[:i + 1] + [''] + fn(t) + [''] + lines[j:]
        print('filled', name)
    open(path, 'wb').write('\r\n'.join(lines).encode('utf-8'))


def check(t, path):
    lines = read_doc(path)
    doc = '\n'.join(lines)
    bad = []
    for name, fn in BLOCKS.items():
        if doc_block(lines, name) != fn(t):
            bad.append('block "%s" is not up to date' % name)
    total = 'סה"כ %d קשתות, %s הפניות.' % (len(t.rows), '{:,}'.format(sum(r[2] for r in t.rows)))
    if total not in doc:
        bad.append('§4 total line, expected: %s' % total)
    mat, _ = t.topic_matrix()
    node = {'MAIN': 'main', 'BLE': 'BLE', 'BTN': 'BTN', 'LOGIC': 'Logic', 'STATES': 'States', 'SCREENS': 'Screens',
            'RENDER': 'Render', 'DB': 'DB', 'PREFS': 'Prefs', 'CLK': 'CLK', 'HW': 'HW', 'META': 'Meta'}
    graph = re.search(r'## 3\..*?```mermaid\n(.*?)```', doc, re.S)
    g = {}
    if graph:
        for m in re.finditer(r'^\s*(\w+) -\.?->\|"?(\d+)[^|]*\| (\w+)', graph.group(1), re.M):
            g[(node[m.group(1)], node[m.group(3)])] = int(m.group(2))
    thr = re.search(r'\*\*קשתות נושא מתחת לסף:\*\*\n\n(.*?)\n\n', doc, re.S)
    tt = {}
    if thr:
        for r in thr.group(1).split('\n')[2:]:
            m = re.match(r'\| `(\w+) → (\w+)` \| (\d+) \|', r)
            if m:
                lbl = lambda s: 'Meta' if s == 'Metadata' else s
                tt[(lbl(m.group(1)), lbl(m.group(2)))] = int(m.group(3))
    for k, v in mat.items():
        if v >= 10 and g.get(k) != v:
            bad.append('§3 graph %s -> %s: %s, A.1 %d' % (k[0], k[1], g.get(k), v))
        if v < 10 and tt.get(k) != v:
            bad.append('§3 threshold table %s -> %s: %s, A.1 %d' % (k[0], k[1], tt.get(k), v))
    for k, v in list(g.items()) + [kv for kv in tt.items() if kv[1]]:
        if mat.get(k) != v:
            bad.append('§3 %s -> %s = %d, A.1 %s' % (k[0], k[1], v, mat.get(k)))
    for x in bad:
        print('CHECK FAIL:', x)
    print('check: %s (%d edges, %d references)' % ('OK' if not bad else 'FAILED', len(t.rows), sum(r[2] for r in t.rows)))
    return not bad


# ------------------------------------------------------------------ selfcheck


def old_table(lines, header_prefix):
    i = next(k for k, x in enumerate(lines) if x.startswith(header_prefix))
    out = []
    for x in lines[i:]:
        if not x.strip():
            break
        out.append(x)
    return out


def selfcheck():
    with tempfile.TemporaryDirectory() as tmp:
        tar = subprocess.run(['git', '-C', REPO, 'archive', SELFCHECK_TREE, 'JC6416/src'],
                             capture_output=True, check=True).stdout
        with tarfile.open(fileobj=io.BytesIO(tar)) as tf:
            if hasattr(tarfile, 'data_filter'):
                tf.extractall(tmp, filter='data')
            else:
                tf.extractall(tmp)
        t = Tree(os.path.join(tmp, 'JC6416', 'src'))
    old = subprocess.run(['git', '-C', REPO, 'show', '%s:docs/architecture/dependencies.md' % SELFCHECK_DOC],
                         capture_output=True, check=True).stdout.decode('utf-8').replace('\r\n', '\n').split('\n')
    bad, notes = [], []
    # §4
    o, n = old_table(old, '| # | מ- | אל |'), block_edges(t)
    if len(o) != len(n):
        bad.append('§4 rows %d vs %d' % (len(o), len(n)))
    ids_ok = {('States/MainStateInfoSets', 'DB/DayInfo'), ('States/MainState', 'DB/DayInfo')}
    ties = 0
    for x, y in zip(o[2:], n[2:]):
        cx, cy = x.split('|'), y.split('|')
        if [cx[i] for i in (1, 2, 3, 4, 7)] != [cy[i] for i in (1, 2, 3, 4, 7)]:
            bad.append('§4 row differs: %s' % x)
        if cx[5] != cy[5] and (cx[2].strip(' `'), cx[3].strip(' `')) not in ids_ok:
            bad.append('§4 identifier count differs: %s' % x)
        ties += cx[6] != cy[6]
    notes.append('§4: %d rows; examples in another tie order in %d rows' % (len(n) - 2, ties))
    # A.2, A.3: identical
    for name, head in (('a2', '| States/ |'), ('a3', '| Screens/ |')):
        if old_table(old, head) != BLOCKS[name](t):
            bad.append('%s differs' % name.upper())
    # A.1: identical except Prefs -> States `inc`
    o, n = old_table(old, '| מ-\\אל |'), block_a1(t)
    for x, y in zip(o, n):
        if x != y and not (x.startswith('| **Prefs** |') and x.replace('| inc |', '|  |') == y):
            bad.append('A.1 row differs: %s' % x)
    # A.4: identical after normalizing unit names in parentheses, except the UserPreferences include
    o, n = old_table(old, '| יחידה |'), block_a4(t)
    norm = lambda s: re.sub(r'\((\w+): [\w.]+ (\d+)', r'(\1 \2', s)
    for x, y in zip(o, n):
        if x not in (y, norm(y)) and x != '| Preferences/UserPreferences | (include States) |':
            bad.append('A.4 row differs: %s / %s' % (x, y))
    if len(o) != len(n):
        bad.append('A.4 rows %d vs %d' % (len(o), len(n)))
    for x in notes:
        print('selfcheck:', x)
    for x in bad:
        print('SELFCHECK FAIL:', x)
    print('selfcheck: %s (tree %s, document %s)' % ('OK' if not bad else 'FAILED', SELFCHECK_TREE, SELFCHECK_DOC))
    return not bad


# ------------------------------------------------------------------ report


def report(t):
    mat, inc = t.topic_matrix()
    print('edges %d, references %d' % (len(t.rows), sum(r[2] for r in t.rows)))
    print('\n# topic edges')
    for (a, b), n in sorted(mat.items(), key=lambda kv: (-kv[1], kv[0])):
        print('  %-8s -> %-8s %5d' % (a, b, n))
    print('  include-only topic pairs:', sorted(inc))
    print('\n# marked edges (§4 row number)')
    for i, (a, b, n, _, _) in enumerate(t.rows, 1):
        if mark(a, b):
            print('  %3d %-28s -> %-28s %4d %s' % (i, a, b, n, mark(a, b)))
    print('\n# per unit, outgoing to other topics')
    out = defaultdict(lambda: defaultdict(int))
    for (a, b), labels in t.edges.items():
        out[a][topic(b)] += sum(labels.values())
    for a in sorted(out):
        print('  %-30s %s' % (a, ', '.join('%s %d' % kv for kv in sorted(out[a].items(), key=lambda kv: -kv[1]))))


# ------------------------------------------------------------------ main


def main(argv):
    args = argv[1:]
    if not args or args[0] in ('-h', '--help'):
        print(__doc__)
        return 0
    opt = {'--src': os.path.join(REPO, 'JC6416', 'src'),
           '--doc': os.path.join(REPO, 'docs', 'architecture', 'dependencies.md')}
    for k in opt:
        if k in args:
            opt[k] = args[args.index(k) + 1]
    cmd = args[0]
    if cmd == 'selfcheck':
        return 0 if selfcheck() else 1
    t = Tree(opt['--src'])
    if cmd == 'print':
        print('\n'.join(BLOCKS[args[1]](t)))
    elif cmd == 'write':
        write(t, opt['--doc'])
    elif cmd == 'check':
        return 0 if check(t, opt['--doc']) else 1
    elif cmd == 'report':
        report(t)
    else:
        print(__doc__)
        return 2
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
