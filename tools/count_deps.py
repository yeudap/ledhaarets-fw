r"""
count_deps.py — unit-to-unit dependency counter for one model's src/ tree. Read-only.

Implements the counting rules of docs/architecture/dependencies.md §1:
  * unit = .h/.cpp pair, with the exceptions listed there (CoreProtocol.h, FirmwareProtocol.h,
    Omer.cpp, metadata.cpp, Screens.h, States.h, DataBase/*, Fonts/*);
  * a reference = an identifier in code that belongs to another unit (class, free function,
    global, macro, enumerator, file-scope const). Comments, strings and #include lines are not
    counted;
  * a global counts toward the unit that defines its TYPE; a global of a basic or external type
    counts toward the unit whose header declares it (else the defining unit);
  * access chains (bl.mainState.numOfValidTexts) count once per unit they touch; a method call
    counts toward the unit that implements the method;
  * manual exclusions: local shadowing of `now` / `place`, Place::orHachaim vs class orHachaim,
    Metadata counted only through `Metadata::`, the `DataPrefs` fields `place` / `zmanim` vs the
    global `place` (DB/Place) and the array `zmanim` (DB/Shita);
  * free functions defined only in a .cpp (not declared in any header) are internal to it;
  * access through a preference view (stage 2a; units in VIEW_UNITS, e.g. `dataPrefs.zmanim.shita`)
    counts once toward the view unit (`Preferences/DataPrefs`), with the chain up to the
    preference as identifier. It does not count toward `Preferences/UserPreferences`, also when a
    method is called on the preference. The view globals (`inline constexpr <Struct> <name>{...}`)
    are recognized only in the view units.

The parser is heuristic (regex + brace matching, no preprocessor, no real C++ front end).
Validation: on JC6416 at commit 9c75812 it reproduces all 374 edges of the previous map
(dependencies.md §4 at commit e7f8efb) with the exact reference counts (6,747 in total); the
identifier count matches in 372 of them.
After the view support (2a.2.8): output identical to the previous version on trees without views
(JC4020 and JC6432 at e7f8efb, JC6416 at e76aaea). On JC6416 at e7f8efb, per consumer and per
preference, references to UserPreferences plus the views equal the previous version's references
to UserPreferences at c6882d6 (before 2a.2), except the 7.08 changes (`dayIndex` -1, four F11
reads +4).
After the DataPrefs name-collision exclusions (2a.2.10): output identical to the previous version
on JC4020 and JC6432 at 72b432e and on JC6416 at e76aaea; on JC6416 at 72b432e the only change is
that `Preferences/DataPrefs -> DB/Place` and `-> DB/Shita` (1 reference each) are gone.
The generated blocks of dependencies.md are written from this output by tools/gen_deps_doc.py.
It is tuned to this code base. Its main use is comparing two trees (before / after a production
sync) with --compare. --includes is a rough helper (include edges with no reference behind them).

Usage (Python 3.8+, no packages needed; run from anywhere):

    python -I tools/count_deps.py <src_dir>                      # edge list, strongest first
    python -I tools/count_deps.py <src_dir> --edge FROM TO       # one edge, all identifiers
    python -I tools/count_deps.py <src_dir> --includes           # include-only edges
    python -I tools/count_deps.py <old_src> --compare <new_src>  # edges that changed
    python -I tools/count_deps.py <src_dir> --json               # machine-readable

Example — what changed between an earlier commit and the working tree, for JC6416
(Git Bash, from the repo root; <dir> is any empty folder outside the repo):

    git archive 9c75812 JC6416/src | tar -x -C <dir>
    python -I tools/count_deps.py <dir>/JC6416/src --compare JC6416/src

Only reads files. Writes nothing.
"""
import os
import re
import sys
import json
from collections import defaultdict, OrderedDict

# --------------------------------------------------------------------------- units

SRC_EXT = ('.h', '.hpp', '.cpp', '.c')

# Units that define the per-layer preference views (stage 2a). Each holds one global, declared
# `inline constexpr <Struct> <name>{...};`, whose members are references to `userPreferences` fields.
VIEW_UNITS = {'Preferences/SystemPrefs', 'Preferences/UserContent', 'Preferences/PresentationPrefs',
              'Preferences/DisplayPrefs', 'Preferences/LogicPrefs', 'Preferences/DataPrefs'}


def unit_of(rel):
    rel = rel.replace('\\', '/')
    low = rel.lower()
    if low.startswith('db/database/'):
        return 'DataBase'
    if low.startswith('fonts/'):
        return 'Fonts'
    special = {
        'ble/coreprotocol.h': 'BLE/CoreService',
        'ble/firmwareprotocol.h': 'BLE/FirmwareService',
        'db/omer.cpp': 'DB/DayInfo',
        'metadata.cpp': 'Metadata',
        'screens/screens.h': 'Screens/Screens.h',
        'states/states.h': 'States/States.h',
    }
    if low in special:
        return special[low]
    return os.path.splitext(rel)[0]


# --------------------------------------------------------------------------- lexing

def strip(text):
    """Remove comments and string/char literal contents; keep line structure."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if c in '"\'':
            q = c
            j = i + 1
            while j < n and text[j] != q and text[j] != '\n':
                j += 2 if text[j] == '\\' else 1
            out.append(q + q)
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


def drop_preproc(text, keep_define_bodies=True):
    """Blank #include / #if / #pragma lines; keep #define bodies (they hold references)."""
    lines = text.split('\n')
    res = []
    cont = False
    for ln in lines:
        s = ln.lstrip()
        if cont:
            res.append(ln if keep_define_bodies else '')
            cont = ln.rstrip().endswith('\\')
            continue
        if s.startswith('#'):
            m = re.match(r'#\s*define\s+(\w+)(\([^)]*\))?(.*)', s)
            if m and keep_define_bodies:
                res.append(' ' * (len(ln) - len(m.group(3) or '')) + (m.group(3) or ''))
            else:
                res.append('')
            cont = ln.rstrip().endswith('\\')
            continue
        res.append(ln)
    return '\n'.join(res)


def match_brace(text, i, open_c='{', close_c='}'):
    """text[i] == open_c; return index of matching close_c."""
    d = 0
    n = len(text)
    while i < n:
        c = text[i]
        if c == open_c:
            d += 1
        elif c == close_c:
            d -= 1
            if d == 0:
                return i
        i += 1
    return n - 1


IDENT = re.compile(r'[A-Za-z_]\w*')
KEYWORDS = set('''alignas alignof and asm auto bool break case catch char char16_t char32_t class
const constexpr const_cast continue decltype default delete do double dynamic_cast else enum
explicit export extern false float for friend goto if inline int long mutable namespace new
noexcept not nullptr operator or private protected public register reinterpret_cast return short
signed sizeof static static_assert static_cast struct switch template this throw true try typedef
typeid typename union unsigned using virtual void volatile wchar_t while override final
uint8_t int8_t uint16_t int16_t uint32_t int32_t uint64_t int64_t size_t double_t String
NULL'''.split())
QUALIFIERS = {'const', 'static', 'inline', 'extern', 'constexpr', 'volatile', 'mutable',
              'unsigned', 'signed', 'long', 'short', 'virtual', 'explicit', 'friend', 'typename',
              'struct', 'class', 'enum', 'register'}


# --------------------------------------------------------------------------- declarations

class Model:
    def __init__(self):
        self.classes = {}        # name -> dict(unit, fields{name:type}, methods{name:set(units)}, bases[])
        self.enums = {}          # enum name -> unit
        self.enumerators = {}    # enumerator -> unit
        self.macros = {}         # macro -> unit
        self.namespaces = {}     # namespace -> unit
        self.aliases = {}        # typedef/using -> unit
        self.consts = {}         # file-scope const -> unit
        self.func_decl = defaultdict(set)   # free function -> units declaring it in a header
        self.func_def = defaultdict(set)    # free function -> units defining it
        self.func_proto = defaultdict(set)  # free function -> .cpp units with a prototype
        self.var_decl = defaultdict(list)   # global -> [(unit, is_header, is_extern, type)]
        self.method_impl = defaultdict(set)  # (class, method) -> units


def decl_parts(head):
    """Split a declaration head into (type_name, name, after_name_char)."""
    toks = re.findall(r'::|[A-Za-z_]\w*|<|>|\*|&|\[|\]|\(|\)|=|,|[^\s\w]', head)
    return toks


def base_type(head_before_name):
    """First non-qualifier identifier of a declaration's type part (template args ignored)."""
    s = re.sub(r'<[^<>]*(<[^<>]*>[^<>]*)*>', ' ', head_before_name)
    for t in IDENT.findall(s):
        if t in QUALIFIERS:
            continue
        return t
    return None


def looks_like_params(inner):
    inner = inner.strip()
    if inner == '' or inner == 'void':
        return True
    for part in split_top(inner, ','):
        p = re.sub(r'<[^<>]*>', ' ', part.split('=')[0])
        p = re.sub(r'\w+\s*::\s*', '', p)          # qualified names count as one token
        ids = [t for t in IDENT.findall(p)]
        if re.search(r'\d', p.replace('int8_t', '').replace('int16_t', '').replace('int32_t', '')
                     .replace('int64_t', '')) and len(ids) < 2:
            return False
        if len(ids) >= 2 or (len(ids) == 1 and ids[0] in KEYWORDS):
            continue
        return False
    return True


def split_top(s, sep):
    parts, d, cur = [], 0, []
    for c in s:
        if c in '([{<':
            d += 1
        elif c in ')]}>':
            d -= 1
        if c == sep and d == 0:
            parts.append(''.join(cur))
            cur = []
        else:
            cur.append(c)
    parts.append(''.join(cur))
    return parts


def parse_scope(text, unit, is_header, m, cls=None, ns=None):
    """Parse declarations in a scope (file / namespace / class body)."""
    i, n = 0, len(text)
    start = 0
    while i < n:
        c = text[i]
        if c == '(':
            i = match_brace(text, i, '(', ')') + 1
            continue
        if c == '{':
            head = text[start:i].strip()
            end = match_brace(text, i)
            body = text[i + 1:end]
            nxt = end + 1
            handle_block(head, body, unit, is_header, m, cls, ns)
            # skip trailing declarators / ';'
            if re.match(r'\s*;', text[nxt:]):
                nxt = text.index(';', nxt) + 1
            elif re.match(r'\s*\w[\w\s,*]*;', text[nxt:]) and re.match(r'(struct|class|enum|union)\b', head):
                nxt = text.index(';', nxt) + 1
            i = start = nxt
            continue
        if c == ';':
            head = text[start:i].strip()
            handle_stmt(head, unit, is_header, m, cls, ns)
            i = start = i + 1
            continue
        if c == ':' and cls is not None:
            h = text[start:i].strip()
            if h in ('public', 'private', 'protected'):
                i = start = i + 1
                continue
        i += 1


def handle_block(head, body, unit, is_header, m, cls, ns):
    h = re.sub(r'^template\s*<[^{]*?>\s*', '', head)
    mc = re.match(r'(?:typedef\s+)?(class|struct|union)\s*(?:\w+\s+(?=\w))?(\w+)?\s*(?:final\s*)?(?::\s*(.*))?$', h, re.S)
    if mc and '(' not in h:
        name = mc.group(2)
        bases = [b.strip().split()[-1] for b in (mc.group(3) or '').split(',') if b.strip()]
        bases = [re.sub(r'<.*', '', b) for b in bases]
        if name:
            c = m.classes.setdefault(name, {'unit': unit, 'fields': {}, 'methods': defaultdict(set), 'bases': []})
            c['unit'] = unit
            c['bases'] = bases
            parse_scope(body, unit, is_header, m, cls=name, ns=ns)
        return
    me = re.match(r'(?:typedef\s+)?enum\s*(class|struct)?\s*(\w+)?', h)
    if me and h.startswith(('enum', 'typedef enum')):
        scoped = bool(me.group(1))
        name = me.group(2)
        if name:
            if cls:
                m.classes[cls]['fields'][name] = None
            m.enums.setdefault(name, unit)   # nested enum types too (MessageScreen::MessageType)
        if scoped:
            return  # enumerators are reached only as Name::X
        for part in split_top(body, ','):
            e = IDENT.match(part.strip())
            if e:
                if cls:
                    m.classes[cls]['fields'][e.group(0)] = None
                else:
                    m.enumerators[e.group(0)] = unit
        return
    mn =re.match(r'namespace\s+(\w+)', h)
    if mn:
        m.namespaces[mn.group(1)] = unit
        parse_scope(body, unit, is_header, m, cls=None, ns=mn.group(1))
        return
    if re.match(r'extern\s*""', h):
        parse_scope(body, unit, is_header, m, cls, ns)
        return
    if '=' in split_paren_free(h):
        handle_stmt(h, unit, is_header, m, cls, ns, has_init=True)
        return
    if unit in VIEW_UNITS and not cls and not ns and '(' not in h:
        # view global: `inline constexpr DataPrefs dataPrefs{...};` (brace init, no `=`).
        # Registered as a plain global, not as a const, so that access chains are followed.
        mv = re.match(r'(.*\b)([A-Za-z_]\w*)$', h, re.S)
        if mv and base_type(mv.group(1)):
            m.var_decl[mv.group(2)].append((unit, is_header, False, base_type(mv.group(1))))
        return
    # function definition
    fm = re.search(r'([\w:~]+(?:\s*operator\s*\S+)?)\s*\(', h)
    if fm:
        name = fm.group(1)
        if '::' in name:
            cl, meth = name.rsplit('::', 1)
            cl = cl.split('::')[-1]
            m.method_impl[(cl, meth)].add(unit)
        elif cls:
            m.classes[cls]['methods'][name].add(unit)
        elif ns:
            pass
        else:
            if name not in KEYWORDS:
                m.func_def[name].add(unit)
                if is_header:
                    m.func_decl[name].add(unit)


def split_paren_free(h):
    out, d = [], 0
    for c in h:
        if c == '(':
            d += 1
        elif c == ')':
            d -= 1
        elif d == 0:
            out.append(c)
    return ''.join(out)


def handle_stmt(head, unit, is_header, m, cls, ns, has_init=False):
    h = head.strip()
    if not h:
        return
    h = re.sub(r'^template\s*<[^;]*?>\s*', '', h)
    if h.startswith(('using namespace', 'return', 'static_assert', 'friend')):
        return
    mt = re.match(r'typedef\s+.*?(\w+)\s*(\[[^\]]*\])?$', h, re.S)
    if mt:
        m.aliases[mt.group(1)] = unit
        return
    mu = re.match(r'using\s+(\w+)\s*=', h)
    if mu:
        m.aliases[mu.group(1)] = unit
        return
    if re.match(r'(class|struct|union|enum)\s+\w+$', h):
        return  # forward declaration
    flat = split_paren_free(h)
    if '(' in h and '=' not in flat.split('{')[0] and not has_init:
        # function declaration or ctor-style variable
        fm = re.search(r'([\w:~]+)\s*\(', h)
        if not fm:
            return
        name = fm.group(1)
        inner_start = fm.end() - 1
        inner = h[inner_start + 1:match_brace(h, inner_start, '(', ')')]
        before = h[:fm.start()]
        if before.strip() and base_type(before) and not looks_like_params(inner) and '::' not in name:
            add_var(name, before, unit, is_header, m, cls, ns, h)
            return
        if cls:
            m.classes[cls]['methods'][name.split('::')[-1]].add(unit)   # declared; inline body unknown
            return
        if ns or '::' in name or name in KEYWORDS or not before.strip():
            return
        if is_header:
            m.func_decl[name].add(unit)
        else:
            m.func_proto[name].add(unit)   # e.g. `extern void MorphMulty(...)` in MainState.cpp
        return
    # variable declaration(s)
    decls = split_top(h, ',')
    first = decls[0]
    nm = re.match(r'(.*?)([A-Za-z_]\w*)\s*(\[[^=]*\])*\s*(=.*|\{.*)?$', first, re.S)
    if not nm:
        return
    before = nm.group(1)
    if not base_type(before):
        return
    add_var(nm.group(2), before, unit, is_header, m, cls, ns, h)
    for d in decls[1:]:
        d2 = re.match(r'\s*[*&]*\s*([A-Za-z_]\w*)', d)
        if d2:
            add_var(d2.group(1), before, unit, is_header, m, cls, ns, h)


def add_var(name, before, unit, is_header, m, cls, ns, full):
    t = base_type(before)
    if cls:
        # Namespace-qualified member types (std::vector<...>) are not followed in chains:
        # `userPreferences.simplePrefs` counts as `userPreferences`.
        m.classes[cls]['fields'][name] = ('EXT:' + str(t)) if '::' in before else t
        return
    if ns:
        return
    if name in KEYWORDS:
        return
    if re.search(r'\bstatic\b', before) and not is_header:
        return  # file-local global (e.g. activeClock in Clock.cpp)
    is_extern = bool(re.search(r'\bextern\b', before))
    is_const = bool(re.search(r'\b(const|constexpr)\b', before)) and '*' not in before.split('const')[-1]
    m.var_decl[name].append((unit, is_header, is_extern, t))
    if is_const and not is_extern:
        m.consts.setdefault(name, unit)


# --------------------------------------------------------------------------- model build

def load_tree(src):
    files = []
    for root, _, fs in os.walk(src):
        for f in fs:
            if f.endswith(SRC_EXT):
                p = os.path.join(root, f)
                rel = os.path.relpath(p, src).replace('\\', '/')
                with open(p, encoding='utf-8-sig', errors='replace') as fh:
                    raw = fh.read()
                files.append((rel, unit_of(rel), raw))
    return files


def build_model(files):
    m = Model()
    for rel, unit, raw in files:
        txt = strip(raw)
        is_header = rel.endswith(('.h', '.hpp'))
        # macros (skip include guards)
        guards = set(g for g in re.findall(r'#\s*ifndef\s+(\w+)\s*\n\s*#\s*define\s+\1[ \t\r]*\n', txt)
                     if re.search(r'_H_?$|^_', g))
        for mm in re.finditer(r'^\s*#\s*define\s+(\w+)', txt, re.M):
            if mm.group(1) not in guards:
                m.macros.setdefault(mm.group(1), unit)
        if unit in ('DataBase', 'Fonts'):
            # data units: only file-scope names
            for vm in re.finditer(r'^\s*(?:const\s+)?(?:static\s+)?(?:PROGMEM\s+)?[\w:]+\s*\*?\s*(?:const\s+)?(\w+)\s*(?:\[[^\]]*\])+\s*(?:PROGMEM\s*)?=', txt, re.M):
                m.consts.setdefault(vm.group(1), unit)
                m.var_decl[vm.group(1)].append((unit, True, False, None))
            for vm in re.finditer(r'^\s*(?:const\s+)?GFXfont\s+(\w+)', txt, re.M):
                m.consts.setdefault(vm.group(1), unit)
                m.var_decl[vm.group(1)].append((unit, True, False, None))
            continue
        parse_scope(drop_preproc(txt, keep_define_bodies=False), unit, is_header, m)
    # method implementations
    for (cl, meth), units in m.method_impl.items():
        if cl in m.classes:
            m.classes[cl]['methods'][meth] |= units
    return m


def class_unit(m, t):
    if t in m.classes:
        return m.classes[t]['unit']
    if t in m.enums:
        return m.enums[t]
    if t in m.aliases:
        return m.aliases[t]
    return None


def global_owner(m, name):
    """Owner unit and type of a global variable."""
    decls = m.var_decl.get(name)
    if not decls:
        return None, None
    t = next((d[3] for d in decls if d[3]), None)
    tu = class_unit(m, t) if t else None
    if tu:
        return tu, t
    hdr = [d[0] for d in decls if d[1]]
    if hdr:
        return hdr[0], t
    defs = [d[0] for d in decls if not d[2]]
    if defs:
        return defs[0], t
    return decls[0][0], t


def find_member(m, cls, name, seen=None):
    seen = seen or set()
    if cls not in m.classes or cls in seen:
        return None
    seen.add(cls)
    c = m.classes[cls]
    if name in c['methods']:
        units = c['methods'][name]
        impl = [u for u in units if u != c['unit']]
        return ('method', sorted(impl)[0] if impl else c['unit'], None)
    if name in c['fields']:
        t = c['fields'][name]
        if t and t.startswith('EXT:'):
            return None
        return ('field', c['unit'], t)
    for b in c['bases']:
        r = find_member(m, b, name, seen)
        if r:
            return r
    return None


# --------------------------------------------------------------------------- references

# Names that collide with standard / Arduino functions (Zman.h overloads min/max); not counted.
STD_NAMES = {'min', 'max', 'abs'}

EXCLUDE = {
    # unit -> identifiers that are local shadows / name collisions there (dependencies.md §1)
    'DB/DayInfo': {'now', 'place'},
    'BLE/OtaProtocol': {'now'},
    'DB/DB': {'place'},
    'DB/Sun': {'place'},
    'DB/Place': {'orHachaim', 'kiseRachamim'},
    # struct fields `PlacePrefs place;` / `ZmanimPrefs zmanim;` (DataPrefs.h), not the global
    # `place` (DB/Place) or the array `zmanim` (DB/Shita)
    'Preferences/DataPrefs': {'place', 'zmanim'},
}


def count_refs(files, m):
    edges = defaultdict(lambda: defaultdict(int))   # (from, to) -> label -> count
    file_units = set(u for _, u, _ in files)
    for rel, unit, raw in files:
        if unit in ('DataBase', 'Fonts'):
            continue
        txt = drop_preproc(strip(raw))
        excl = EXCLUDE.get(unit, set())
        n = len(txt)
        for tm in IDENT.finditer(txt):
            tok = tm.group(0)
            s, e = tm.start(), tm.end()
            pre = txt[max(0, s - 3):s].rstrip()
            if pre.endswith('.') or pre.endswith('->') or pre.endswith('::'):
                continue
            if tok in KEYWORDS or tok in excl or tok in STD_NAMES:
                continue
            rest = txt[e:e + 3].lstrip()
            hit = OrderedDict()
            if rest.startswith('::'):
                q = re.match(r'\s*::\s*(~?\w+)', txt[e:])
                qn = q.group(1) if q else ''
                if tok in m.namespaces:
                    hit[m.namespaces[tok]] = tok
                else:
                    u = class_unit(m, tok)
                    if u:
                        hit[u] = tok
            elif tok in m.var_decl and rest.startswith('('):
                # `DateTime now();` declares a method, it is not the global `now`;
                # a call like `= TimeToDecimalDay(...)` (parsed as a ctor-style global) still counts
                if not re.search(r'[\w*&>]\s*$', txt[max(0, s - 40):s]):
                    u, _ = global_owner(m, tok)
                    if u:
                        hit[u] = tok
            elif tok in m.var_decl and tok not in m.consts and tok not in m.classes:
                u, t = global_owner(m, tok)
                if u:
                    hit[u] = tok
                    label = tok
                    view_root = u in VIEW_UNITS
                    indexed = False
                    j = e
                    cur = t
                    while cur:
                        k = j
                        while True:
                            mm = re.match(r'\s*', txt[k:])
                            k += mm.end()
                            if k < n and txt[k] == '[':
                                k = match_brace(txt, k, '[', ']') + 1
                                indexed = True
                                continue
                            break
                        acc = re.match(r'\s*(\.|->)\s*(\w+)', txt[k:])
                        if not acc:
                            break
                        mem = acc.group(2)
                        r = find_member(m, cur, mem)
                        if not r:
                            break
                        kind, mu, mt = r
                        j = k + acc.end()
                        direct = (label == tok and not indexed)
                        if kind == 'method':
                            # id = chain up to the method: for a method called directly on the
                            # global (bl.configStates), or one implemented in a new unit
                            if mu not in hit or direct:
                                hit[mu] = label + '.' + mem
                                hit.move_to_end(mu)
                            break
                        if mt is None:
                            # enumerator / nested enum (messageScreen.SUCCESS): same unit, no new id
                            hit.setdefault(mu, label)
                            break
                        if indexed and mu in hit:
                            pass   # shitot[i]->name stays `shitot`
                        else:
                            label = label + '.' + mem
                            hit[mu] = label
                            hit.move_to_end(mu)
                        if view_root and class_unit(m, mt) not in VIEW_UNITS:
                            # access through a view ends at the preference it exposes
                            # (dataPrefs.zmanim.shita): one reference to the view unit, none
                            # to UserPreferences, also when a method is called on it
                            break
                        cur = mt if mt in m.classes else None
            else:
                u = (m.macros.get(tok) or m.enumerators.get(tok) or class_unit(m, tok)
                     or m.consts.get(tok))
                defs = m.func_def.get(tok, set())
                if not u and tok in m.func_decl:
                    u = sorted(defs)[0] if defs else sorted(m.func_decl[tok])[0]
                if not u and defs and (m.func_proto.get(tok, set()) - defs):
                    u = sorted(defs)[0]
                if u:
                    hit[u] = tok
            for u, label in hit.items():
                if u != unit and u in file_units:
                    edges[(unit, u)][label] += 1
    return edges


def count_includes(files):
    by_name = {}
    for rel, unit, _ in files:
        by_name[rel.lower()] = unit
        by_name[os.path.basename(rel).lower()] = by_name.get(os.path.basename(rel).lower(), unit)
    inc = defaultdict(list)
    for rel, unit, raw in files:
        for ln_no, ln in enumerate(raw.split('\n'), 1):
            mm = re.match(r'\s*#\s*include\s*"([^"]+)"', ln)
            if not mm:
                continue
            target = mm.group(1).replace('\\', '/')
            base = os.path.normpath(os.path.join(os.path.dirname(rel), target)).replace('\\', '/').lower()
            tu = by_name.get(base) or by_name.get(target.lower()) or by_name.get(os.path.basename(target).lower())
            if tu and tu != unit:
                inc[(unit, tu)].append('%s:%d' % (rel, ln_no))
    return inc


# --------------------------------------------------------------------------- output

def summarize(edges):
    rows = []
    for (a, b), labels in edges.items():
        total = sum(labels.values())
        top = sorted(labels.items(), key=lambda kv: (-kv[1], kv[0]))
        rows.append((a, b, total, len(labels), top))
    rows.sort(key=lambda r: (-r[2], r[0], r[1]))
    return rows


def fmt_top(top, k=3):
    return ', '.join('`%s`%s' % (l, '' if c == 1 else '×%d' % c) for l, c in top[:k])


def analyze(src):
    files = load_tree(src)
    m = build_model(files)
    return files, m, count_refs(files, m)


def main(argv):
    if len(argv) < 2 or argv[1] in ('-h', '--help'):
        print(__doc__)
        return 0
    src = argv[1]
    files, m, edges = analyze(src)
    if '--compare' in argv:
        other = argv[argv.index('--compare') + 1]
        _, _, edges2 = analyze(other)
        keys = sorted(set(edges) | set(edges2))
        print('| from | to | old | new | delta | identifiers added / changed |')
        print('| --- | --- | --- | --- | --- | --- |')
        out = []
        for k in keys:
            a = edges.get(k, {})
            b = edges2.get(k, {})
            ta, tb = sum(a.values()), sum(b.values())
            if a == b:
                continue
            ch = []
            for l in sorted(set(a) | set(b)):
                if a.get(l, 0) != b.get(l, 0):
                    ch.append('`%s` %+d' % (l, b.get(l, 0) - a.get(l, 0)))
            out.append((k, ta, tb, ch))
        out.sort(key=lambda r: (-abs(r[2] - r[1]), r[0]))
        for (fa, fb), ta, tb, ch in out:
            print('| `%s` | `%s` | %d | %d | %+d | %s |' % (fa, fb, ta, tb, tb - ta, ', '.join(ch)))
        inc1, inc2 = count_includes(files), count_includes(load_tree(other))
        new_inc = sorted(set(inc2) - set(inc1))
        gone_inc = sorted(set(inc1) - set(inc2))
        if new_inc or gone_inc:
            print('\nInclude edges added: %s' % ', '.join('%s -> %s' % k for k in new_inc))
            print('Include edges removed: %s' % ', '.join('%s -> %s' % k for k in gone_inc))
        return 0
    if '--edge' in argv:
        i = argv.index('--edge')
        k = (argv[i + 1], argv[i + 2])
        for l, c in sorted(edges.get(k, {}).items(), key=lambda kv: -kv[1]):
            print('%5d  %s' % (c, l))
        return 0
    if '--includes' in argv:
        inc = count_includes(files)
        for k in sorted(inc):
            if k not in edges:
                print('%s -> %s  (include only: %s)' % (k[0], k[1], ', '.join(inc[k])))
        return 0
    rows = summarize(edges)
    if '--json' in argv:
        print(json.dumps([{'from': a, 'to': b, 'refs': t, 'ids': n, 'labels': dict(top)}
                          for a, b, t, n, top in rows], ensure_ascii=False, indent=1))
        return 0
    print('| # | from | to | refs | ids | examples |')
    print('| --- | --- | --- | --- | --- | --- |')
    for i, (a, b, t, n, top) in enumerate(rows, 1):
        print('| %d | `%s` | `%s` | %d | %d | %s |' % (i, a, b, t, n, fmt_top(top)))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
