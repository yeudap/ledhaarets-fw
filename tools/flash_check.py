"""flash_check.py — flash symbol check for a non-identical image (criterion: refactor-log.md, 2a.3.3,
"Flash check when the image is not identical", and "Decisions"). A filter for surprises, not a proof of
equivalence. Read-only. Every function present in both ELFs whose size changed gets exactly one category;
`manual` means a deviation (stop). Needs the xtensa-esp32 toolchain of PlatformIO (nm, objdump,
c++filt).

Usage (Git Bash, from the repo root; python -B keeps tools/ free of bytecode):
  python -I -B tools/flash_check.py <ref.elf> <cand.elf> <ref_UserPreferences.h> <cand_Preferences_dir>
  python -I -B tools/flash_check.py ... --detail "<demangled function name>"   # diff of one function
  <ref.elf>                 reference build (e.g. a copy of .pio/build/esp32dev/firmware.elf before the step)
  <cand.elf>                candidate build
  <ref_UserPreferences.h>   the reference tree's src/Preferences/UserPreferences.h (git archive of HEAD)
  <cand_Preferences_dir>    the candidate's src/Preferences/ (layer layouts are parsed from the headers)
The layouts are the 2a.3.3 ones (registry with all UserPref members in the reference, layer objects in
the candidate); a later step that changes them (2a.4) must adapt ref_layout / cand_layout first.

History: written for 2a.3.3 as flashcheck2.py (v2, dataflow); moved here without logic changes.
Its base module flashcheck.py (layouts, ELF access, normalization; also v1 of the check, linear
tracking) was merged into this file without logic changes, and v1 removed (docs step after 2a.3.5).

Decoding: recursive descent from the entry (fallthrough, branch / jump / loop targets, jump tables =
words in .flash.rodata that point into the function, which are successors of every `jx`). Bytes no
reachable instruction covers are the function's fill; they must be zero.

Preference addresses: a forward dataflow over the CFG tracks, per register, a known address inside a
preference object (userPreferences and the six layer objects), from `l32r` literals and addmi / addi /
mov on such registers; the state at a join is the intersection of the predecessors' states. Calls clobber
the callee window. Inside UserPreferences members, a2 (this) is userPreferences.

Tokens: base computations produce no token. A load / store through a tracked register names the field
and the UserPref member (`l8ui aX, [ofek._value]`); a tracked address used in any other way, or passed
to a call, is named. Other literals: same symbol+offset, or .rodata with the same bytes (moved), or the
same constant. Branch and table targets are token indexes.

Categories (first level at which ref == cand):
  pref-addressing / two-bases / registry-offset / rodata-moved   (exact tokens; the label says why the
        bytes differ: only preference addressing; the number of preference base loads differs; inside
        a UserPreferences member; a .rodata literal moved with identical bytes)
  alignment        + narrow / wide forms, NOPs (`or a1, a1, a1`), zero fill
  regalloc         + register renaming
  ref-indirection  + the load of userContent.textPrefs (NOT pre-approved)
  manual           anything else
"""
import sys, os, re, struct, bisect, subprocess
from collections import OrderedDict
TC = os.path.expanduser('~/.platformio/packages/toolchain-xtensa-esp32/bin/xtensa-esp32-elf-')

# --------------------------------------------------------------------------- layouts

SIZE = {'uint8_t': (24, 4), 'int8_t': (24, 4), 'uint16_t': (24, 4), 'int16_t': (24, 4),
        'double_t': (40, 8), 'String': (52, 4)}
VALUE = {'uint8_t': (20, 21), 'int8_t': (20, 21), 'uint16_t': (20, 22), 'int16_t': (20, 22),
         'double_t': (24, 32), 'String': (20, 36)}
BASE = {0: 'vptr', 4: '_key', 8: '_type', 12: '_index', 13: '_isModified', 16: '_prefs'}


def userpref_member(t, inner):
    v, d = VALUE[t]
    if inner in BASE:
        return BASE[inner]
    if t == 'String':
        if v <= inner < v + 16:
            return '_value+%d' % (inner - v)
        if d <= inner < d + 16:
            return '_defaultValue+%d' % (inner - d)
    else:
        if inner == v:
            return '_value'
        if inner == d:
            return '_defaultValue'
        if t == 'double_t' and v < inner < v + 8:
            return '_value+%d' % (inner - v)
    return '?%d' % inner


class Layout:
    """fields: list of (offset, size, name, kind) where kind is a UserPref type or 'vector'/'prefs'/'ref'."""

    def __init__(self):
        self.objs = {}

    def add(self, obj, members):
        off, fields, align = 0, [], 4
        for name, kind in members:
            if isinstance(kind, list):           # nested struct
                sub = Layout._struct(kind)
                a = sub[2]
                off = (off + a - 1) // a * a
                for o, s, n, k in sub[0]:
                    fields.append((off + o, s, name + '.' + n, k))
                off += sub[1]
                align = max(align, a)
                continue
            s, a = SIZE.get(kind, {'vector': (12, 4), 'prefs': (8, 4), 'ref': (4, 4)}.get(kind))
            off = (off + a - 1) // a * a
            fields.append((off, s, name, kind))
            off += s
            align = max(align, a)
        size = (off + align - 1) // align * align
        self.objs[obj] = (fields, size)
        return size

    @staticmethod
    def _struct(members):
        l = Layout()
        size = l.add('x', members)
        fields = l.objs['x'][0]
        align = 8 if any(k == 'double_t' for _, _, _, k in fields) else 4
        return fields, size, align

    def resolve(self, obj, off):
        fields, size = self.objs[obj]
        for o, s, n, k in fields:
            if o <= off < o + s:
                inner = off - o
                leaf = n.split('.')[-1]
                if k in VALUE:
                    return '%s.%s' % (leaf, userpref_member(k, inner))
                return '%s+%d' % (leaf, inner)
        return '%s?+%d' % (obj, off)


def parse_members(text, struct):
    body = re.search(r'(?:struct|class) %s\s*\{(.*?)\n\};' % struct, text, re.S).group(1)
    out = []
    for l in body.split('\n'):
        m = re.match(r'^\s+UserPref<(\w+)> &?(\w+);', l)
        if m:
            out.append((m.group(2), m.group(1)))
            continue
        m = re.match(r'^\s+std::vector<UserPrefBase \*> (&?)(\w+);', l)
        if m:
            out.append((m.group(2), 'ref' if m.group(1) else 'vector'))
            continue
        m = re.match(r'^\s+Preferences (\w+);', l)
        if m:
            out.append((m.group(1), 'prefs'))
            continue
        m = re.match(r'^\s+(\w+Prefs) (\w+);', l)
        if m:
            out.append((m.group(2), m.group(1)))
    return out


def ref_layout(hdr):
    t = open(hdr, encoding='utf-8').read().replace('\r\n', '\n')
    L = Layout()
    L.add('userPreferences', parse_members(t, 'UserPreferences'))
    return L


def cand_layout(pdir):
    rd = lambda f: open(os.path.join(pdir, f), encoding='utf-8').read().replace('\r\n', '\n')
    L = Layout()
    d = rd('DataPrefs.h')
    L.add('dataPrefs', [('place', parse_members(d, 'PlacePrefs')), ('zmanim', parse_members(d, 'ZmanimPrefs')),
                        ('minhagim', parse_members(d, 'MinhagimPrefs'))])
    L.add('logicPrefs', parse_members(rd('LogicPrefs.h'), 'LogicPrefs'))
    p = rd('PresentationPrefs.h')
    pm = [x for x in parse_members(p, 'PresentationPrefs') if x[1] != 'LayoutPrefs']
    L.add('presentationPrefs', pm + [('layout', parse_members(p, 'LayoutPrefs'))])
    L.add('displayPrefs', parse_members(rd('DisplayPrefs.h'), 'DisplayPrefs'))
    L.add('systemPrefs', parse_members(rd('SystemPrefs.h'), 'SystemPrefs'))
    L.add('userContent', parse_members(rd('UserContent.h'), 'UserContent'))
    L.add('userPreferences', parse_members(rd('UserPreferences.h'), 'UserPreferences'))
    return L


# --------------------------------------------------------------------------- ELF access

class ElfBase:
    def __init__(self, path, layout):
        self.path, self.layout = path, layout
        out = subprocess.run([TC + 'nm', '-C', '-S', path], capture_output=True, text=True).stdout
        self.syms, objs = {}, []
        for l in out.splitlines():
            m = re.match(r'^([0-9a-f]{8}) ([0-9a-f]{8}) (\S) (.*)$', l)
            if m:
                a, s = int(m.group(1), 16), int(m.group(2), 16)
                self.syms.setdefault(m.group(4), []).append((a, s, m.group(3)))
                if s:
                    objs.append((a, s, m.group(4)))
        objs.sort()
        self.objs, self.starts = objs, [o[0] for o in objs]
        self.data = open(path, 'rb').read()
        self.sections = []
        for l in subprocess.run([TC + 'objdump', '-h', path], capture_output=True, text=True).stdout.splitlines():
            m = re.match(r'\s*\d+\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+[0-9a-f]+\s+([0-9a-f]+)', l)
            if m and int(m.group(3), 16):
                self.sections.append((m.group(1), int(m.group(3), 16), int(m.group(2), 16), int(m.group(4), 16)))
        for name, va, size, off in self.sections:
            if name == '.flash.rodata':
                self.rodata = (va, size, off)
            if name == '.flash.text':
                self.text = (va, size)
        # words in .flash.rodata that point into .flash.text (jump tables)
        va, size, off = self.rodata
        tv, ts = self.text
        self.rod_ptrs = {}
        for i in range(0, size - 3, 4):
            w = struct.unpack_from('<I', self.data, off + i)[0]
            if tv <= w < tv + ts:
                self.rod_ptrs.setdefault(w, []).append(va + i)
        self.ptr_keys = sorted(self.rod_ptrs)
        for o in objs:
            if o[2] in PREF_OBJS:
                pass
        self.pref = {n: self.syms[n][0][0] for n in PREF_OBJS if n in self.syms}

    def read(self, addr, n):
        for name, va, size, off in self.sections:
            if va <= addr < va + size:
                return self.data[off + addr - va: off + addr - va + n]
        return None

    def word(self, addr):
        b = self.read(addr, 4)
        return struct.unpack('<I', b)[0] if b and len(b) == 4 else None

    def sym_at(self, v):
        i = bisect.bisect_right(self.starts, v) - 1
        if i >= 0:
            a, s, n = self.objs[i]
            if a <= v < a + s:
                return n, v - a
        return None, None

    def in_rodata(self, v):
        va, size, _ = self.rodata
        return va <= v < va + size

    def literal(self, v):
        """('pref', obj, off) | ('sym', 'name+off') | ('str', content) | ('const', v)"""
        for n, a in self.pref.items():
            size = self.syms[n][0][1]
            if a <= v < a + size:
                return ('pref', n, v - a)
        n, o = self.sym_at(v)
        if n is not None and not self.in_rodata(v):
            return ('sym', '%s+%d' % (n, o))
        if self.in_rodata(v):
            return ('str', self.rodata_key(v), n)
        return ('const', v)

    def rodata_key(self, v, depth=0):
        """Content of .rodata at v: the string up to NUL when it is text; otherwise the first 3 words,
        each word that points into .rodata or to a symbol replaced by what it points to."""
        b = self.read(v, 64) or b''
        s = b.split(b'\0')[0]
        w0 = self.word(v) if v % 4 == 0 else None
        tv, ts = self.text
        va, vs, _ = self.rodata
        if w0 is not None and (va <= w0 < va + vs or tv <= w0 < tv + ts):
            s = b''           # starts with an address word: compare as words, not as text
        try:
            txt = s.decode('utf-8')
            if s and all(c >= ' ' or c in '\n\r\t' for c in txt):
                return s
        except UnicodeDecodeError:
            pass
        words = []
        for i in range(3):
            w = self.word(v + 4 * i)
            if w is None:
                words.append(None)
            elif self.in_rodata(w) and depth < 1:
                words.append(('->', self.rodata_key(w, depth + 1)))
            else:
                n, o = self.sym_at(w)
                words.append(('sym', n, o) if n is not None and not self.in_rodata(w) else w)
        return ('words', tuple(words))

    # ---------------------------------------------------------------- decoding
    def decode_from(self, start, end):
        out = subprocess.run([TC + 'objdump', '-d', '-z', '--start-address=%d' % start, '--stop-address=%d' % end,
                              self.path], capture_output=True, text=True).stdout
        ins = OrderedDict()
        for l in out.splitlines():
            m = re.match(r'^\s*([0-9a-f]+):\s+([0-9a-f]+)\s+(\S+)\s*(.*)$', l)
            if m:
                a = int(m.group(1), 16)
                ins[a] = (len(m.group(2)) // 2, m.group(3), m.group(4).strip())
        return ins


UNCOND = {'j', 'jx', 'ret', 'ret.n', 'retw', 'retw.n', 'ill', 'ill.n', 'rfe', 'rfi', 'rfwo', 'rfwu'}
PREF_OBJS = ['userPreferences', 'dataPrefs', 'logicPrefs', 'presentationPrefs', 'displayPrefs', 'systemPrefs',
             'userContent']
NOWRITE = re.compile(r'^(s8i|s16i|s32i|s32i\.n|s32c1i|ssi|ssip|b\w*|j|jx|call\w*|ret\w*|entry|loop\w*|memw|extw|nop'
                     r'(\.n)?|isync|dsync|rsync|esync|wsr\w*|wur\w*|xsr\w*|ill(\.n)?|break(\.n)?|witlb|wdtlb|waiti)$')


def target_of(op, args):
    if op.startswith(('b', 'j', 'loop')) and op not in ('jx', 'break', 'break.n'):
        m = re.search(r'([0-9a-f]{8}) <', args)
        if m:
            return int(m.group(1), 16)
    return None


def align_norm(toks):
    out = []
    for t in toks:
        if re.match(r'^or a1, a1, a1$', t) or re.match(r'^nop(\.n)?\b', t):
            continue
        m = re.match(r'^or (a\d+), (a\d+), \2$', t)
        if m:
            t = 'mov %s, %s' % (m.group(1), m.group(2))
        t = re.sub(r'^mov\.n ', 'mov ', t)
        t = re.sub(r'^(\w+)\.n\b', r'\1', t)
        out.append(t)
    return out


def reg_norm(toks):
    return [re.sub(r'\ba\d+\b', 'aR', t) for t in toks]


_ARITY = {}


def arity(mangled):
    """Number of argument registers of a called function, from its demangled signature: `this` for a
    member function, 2 for a double / 64-bit parameter, 1 otherwise. None when unknown (split parts)."""
    if mangled in _ARITY:
        return _ARITY[mangled]
    k = None
    if '$' not in mangled and '.' not in mangled:
        d = subprocess.run([TC + 'c++filt', mangled], capture_output=True, text=True).stdout.strip()
        m = re.match(r'^(.*?)\((.*)\)( const)?$', d)
        if m:
            name, params = m.group(1), m.group(2)
            depth, parts, cur = 0, [], ''
            for c in params:
                if c in '<(':
                    depth += 1
                elif c in '>)':
                    depth -= 1
                if c == ',' and depth == 0:
                    parts.append(cur); cur = ''
                else:
                    cur += c
            if cur.strip():
                parts.append(cur)
            k = sum(2 if re.search(r'\b(double|long long|int64_t|uint64_t)\b', p) and '*' not in p and '&' not in p
                    else 1 for p in parts if p.strip() != 'void')
            if '::' in re.sub(r'<.*>', '', name) and not name.startswith('std::'):
                k += 1     # this (a static member would be over-counted by one register)
        elif d == mangled and not mangled.startswith('_Z'):
            k = None       # C function: signature unknown
    _ARITY[mangled] = k
    return k


NORETURN = re.compile(r'<(__stack_chk_fail|_Unwind_Resume|abort|__cxa_throw|__cxa_rethrow|_ZSt\d+__throw_\w+|'
                      r'__assert_func)(\+0x[0-9a-f]+)?>')


def noreturn(op, args):
    return op.startswith('call') and not op.startswith('callx') and bool(NORETURN.search(args))


class Elf(ElfBase):
    def xtprop(self):
        """Instruction regions from .xt.prop (Xtensa property table, linked addresses):
        sorted list of (start, size, flags); flag 0x2 = instructions, 0x20 = branch target."""
        if hasattr(self, '_xt'):
            return self._xt
        h = subprocess.run([TC + 'objdump', '-h', self.path], capture_output=True, text=True).stdout
        ents = []
        # .xt.prop, and one .xt.prop.<name> per COMDAT (linkonce) function
        for m in re.finditer(r'\s+\d+\s+\.xt\.prop(?:\.\S+)?\s+([0-9a-f]+)\s+[0-9a-f]+\s+[0-9a-f]+\s+([0-9a-f]+)', h):
            size, off = int(m.group(1), 16), int(m.group(2), 16)
            ents += [struct.unpack_from('<III', self.data, off + i) for i in range(0, size - 11, 12)]
        self._xt = sorted(e for e in ents if e[2] & 0x2 and e[1])
        self._xt_starts = [e[0] for e in self._xt]
        return self._xt

    def function(self, name):
        """Decoding by the .xt.prop instruction regions: each region is decoded linearly from its start.
        Bytes outside the regions are the fill. Region starts that no instruction reaches (exception
        landing pads) become roots."""
        if not hasattr(self, '_fcache'):
            self._fcache = {}
        if name in self._fcache:
            return self._fcache[name]
        a, s, _ = self.syms[name][0]
        end = a + s
        xt = self.xtprop()
        i = bisect.bisect_left(self._xt_starts, a)
        regions = []
        while i < len(xt) and xt[i][0] < end:
            st, sz, fl = xt[i]
            regions.append((st, min(st + sz, end), fl))
            i += 1
        reach = OrderedDict()
        for st, en, fl in regions:
            ins = self.decode_from(st, en)
            p = st
            while p < en:
                if p not in ins:
                    raise RuntimeError('%s: region 0x%x decodes off at 0x%x' % (name, st, p))
                reach[p] = ins[p]
                p += ins[p][0]
        body = self.read(a, s)
        covered = set()
        for p, (ln, _, _) in reach.items():
            covered.update(range(p, p + ln))
        fill = bytes(body[x - a] for x in range(a, end) if x not in covered)
        lo = bisect.bisect_left(self.ptr_keys, a)
        hi = bisect.bisect_left(self.ptr_keys, end)
        tables = OrderedDict()
        for t in self.ptr_keys[lo:hi]:
            if t in reach:
                for w in self.rod_ptrs[t]:
                    tables[w] = t
        # roots: region starts not reached by fallthrough, a branch or a table
        targets = set(tables.values()) | {a}
        for p, (ln, op, args) in reach.items():
            t = target_of(op, args)
            if t is not None:
                targets.add(t)
            if op not in UNCOND:
                targets.add(p + ln)
        self.roots = getattr(self, 'roots', {})
        self.roots[name] = [st for st, en, fl in regions if st not in targets]
        r = (a, end, OrderedDict(sorted(reach.items())), fill, tables)
        self._fcache[name] = r
        return r

    def function_heuristic(self, name):
        """The v1 decoder (Elf.function of the former flashcheck.py), plus extra roots: every uncovered run that does not start with zero bytes
        (exception landing pads, reached only through .gcc_except_table) is decoded from its first
        non-zero byte, until the whole function is covered or only zero bytes remain."""
        if not hasattr(self, '_fcache'):
            self._fcache = {}
        if name in self._fcache:
            return self._fcache[name]
        a, s, _ = self.syms[name][0]
        end = a + s
        decoded = {}

        def at(addr):
            if addr not in decoded:
                for k, v in self.decode_from(addr, end).items():
                    decoded.setdefault(k, v)
                if addr not in decoded:
                    raise RuntimeError('%s: cannot decode at 0x%x' % (name, addr))
            return decoded[addr]
        lo = bisect.bisect_left(self.ptr_keys, a)
        hi = bisect.bisect_left(self.ptr_keys, end)
        table_targets = self.ptr_keys[lo:hi]
        body = self.read(a, s)
        reach, roots = {}, []
        work = [a] + table_targets
        while True:
            while work:
                p = work.pop()
                while a <= p < end and p not in reach:
                    ln, op, args = at(p)
                    reach[p] = (ln, op, args)
                    t = target_of(op, args)
                    if t is not None and a <= t < end:
                        work.append(t)
                    if op in UNCOND:
                        break
                    p += ln
            covered = set()
            for p, (ln, _, _) in reach.items():
                covered.update(range(p, p + ln))
            nxt = None
            x = a
            while x < end:
                if x not in covered and body[x - a] != 0 and (x == a or (x - 1) in covered or body[x - 1 - a] == 0):
                    nxt = x
                    break
                x += 1
            if nxt is None:
                break
            roots.append(nxt)
            work = [nxt]
        gaps = [x for x in range(a, end) if x not in covered]
        fill = bytes(body[x - a] for x in gaps)
        tables = OrderedDict()
        for t in table_targets:
            for w in self.rod_ptrs[t]:
                tables[w] = t
        self.roots = getattr(self, 'roots', {})
        self.roots[name] = roots
        r = (a, end, OrderedDict(sorted(reach.items())), fill, tables)
        self._fcache[name] = r
        return r

    def cfg(self, name):
        a, end, reach, fill, tables = self.function(name)
        succ = {}
        tabt = list(dict.fromkeys(tables.values()))
        for p, (ln, op, args) in reach.items():
            s = []
            t = target_of(op, args)
            if t is not None and a <= t < end:
                s.append(t)
            if op == 'jx':
                s += tabt
            if op not in UNCOND and not noreturn(op, args) and p + ln in reach:
                s.append(p + ln)
            succ[p] = s
        return a, end, reach, fill, tables, succ


def resolve(E, b):
    o, off = b
    if o == '@textPrefs':
        return 'textPrefs+%d' % off
    return E.layout.resolve(o, off)


def transfer(E, p, ins, state, opts, emit=None):
    ln, op, args = ins
    st = dict(state)
    ops = [x.strip() for x in args.split(',')] if args else []
    dst = ops[0] if ops else None
    tok = None
    if op == 'l32r':
        m = re.match(r'(a\d+), ([0-9a-f]+)', args)
        v = E.word(int(m.group(2), 16))
        lit = E.literal(v)
        r = m.group(1)
        if lit[0] == 'pref':
            st[r] = (lit[1], lit[2])
        else:
            st.pop(r, None)
            if lit[0] == 'str':
                tok = 'l32r %s, STR:%r' % (r, lit[1])
            elif lit[0] == 'sym':
                tok = 'l32r %s, %s' % (r, lit[1])
            else:
                tok = 'l32r %s, 0x%x' % (r, lit[1])
    elif op in ('addmi', 'addi', 'addi.n') and len(ops) == 3 and ops[1] in st:
        o, off = st[ops[1]]
        st[ops[0]] = (o, off + int(ops[2], 0))
    elif op in ('mov.n', 'mov') and len(ops) == 2 and ops[1] in st:
        st[ops[0]] = st[ops[1]]
    elif op == 'or' and len(ops) == 3 and ops[1] == ops[2] and ops[1] in st:
        st[ops[0]] = st[ops[1]]
    elif op in ('s32i', 's32i.n') and len(ops) == 3 and ops[1] == 'a1' and ops[1] not in st:
        # spill to the stack frame: remember a preference address, forget anything else in that slot
        slot = ('sp', int(ops[2], 0))
        if ops[0] in st:
            st[slot] = st[ops[0]]
        else:
            st.pop(slot, None)
            tok = '%s %s' % (op, args)
    elif op in ('l32i', 'l32i.n') and len(ops) == 3 and ops[1] == 'a1' and ('sp', int(ops[2], 0)) in st:
        st[ops[0]] = st[('sp', int(ops[2], 0))]          # reload of a spilled preference address
    elif re.match(r'^(l8ui|l16ui|l16si|l32i|l32i\.n|s8i|s16i|s32i|s32i\.n|lsi|ssi)$', op) and len(ops) == 3 \
            and ops[1] in st:
        o, off = st[ops[1]]
        field = resolve(E, (o, off + int(ops[2], 0)))
        is_store = op.startswith('s')
        if is_store and ops[0] in st:
            tok = '%s &%s, [%s]' % (op, resolve(E, st[ops[0]]), field)
        else:
            tok = '%s %s, [%s]' % (op, ops[0], field)
        if not is_store:
            st.pop(ops[0], None)
            if o == 'userContent' and field == 'textPrefs+0' and op.startswith('l32i') and opts.get('indirection'):
                st[ops[0]] = ('@textPrefs', 0)
                tok = None
    else:
        writes = bool(dst and re.match(r'^a\d+$', dst) and not NOWRITE.match(op))
        srcs = ops[1:] if writes else ops
        used = [r for r in srcs if r in st]
        t = target_of(op, args)
        a2 = re.sub(r'[0-9a-f]{8} <[^>]*>', 'T', args)
        tok = '%s %s' % (op, a2)
        if op.startswith('call') and not op.startswith('callx'):
            mm = re.search(r'<([^>+]*)', args)
            tok = '%s <%s>' % (op, mm.group(1) if mm else args)
        if op.startswith('call'):
            n = 4 if op.endswith('4') else 8 if op.endswith('8') else 12 if op.endswith('12') else 0
            if n:
                regs = sorted((r for r in st if isinstance(r, str)), key=lambda x: int(x[1:]))
                mm = re.search(r'<([^>+]*)', args)
                k = arity(mm.group(1)) if mm and not op.startswith('callx') else None
                # argument registers of the callee: a(n+2) .. a(n+2+k-1); unknown arity: none counted
                au = [] if not k else ['%s=&%s' % (r, resolve(E, st[r])) for r in regs
                                       if n + 2 <= int(r[1:]) < n + 2 + k]
                if au:
                    tok += ' ARGS{%s}' % ','.join(au)
                for r in regs:
                    if int(r[1:]) >= n:
                        st.pop(r)
        elif used:
            tok += ' {%s}' % ','.join('%s=&%s' % (r, resolve(E, st[r])) for r in used)
        if t is not None:
            tok += ' ->%x' % t
        if writes:
            st.pop(dst, None)
    return st, tok


def analyze(E, fn, opts):
    a, end, reach, fill, tables, succ = E.cfg(fn)
    init = {'a2': ('userPreferences', 0)} if fn.startswith('UserPreferences::') else {}
    IN = {a: init}
    for t in tables.values():
        pass
    work = [a]
    order = list(reach)
    # iterate to a fixed point (intersection at joins)
    changed = True
    INs = {p: None for p in reach}
    INs[a] = init
    roots = list(E.roots.get(fn, []))
    # landing pads (reached through the unwinder from a call): their entry state is the intersection of
    # the states after every call of the function (the caller's window registers survive the unwind)
    while changed:
        changed = False
        call_outs = []
        for p in order:
            if INs[p] is None:
                continue
            out, _ = transfer(E, p, reach[p], INs[p], opts)
            if reach[p][1].startswith('call'):
                call_outs.append(out)
            for s in succ[p]:
                if INs[s] is None:
                    INs[s] = dict(out)
                    changed = True
                else:
                    new = {k: v for k, v in INs[s].items() if out.get(k) == v}
                    if new != INs[s]:
                        INs[s] = new
                        changed = True
        if not changed:
            # instructions still unreached that start a block (after a jump, return or no-return call)
            # are landing pads too
            prev = None
            for p in order:
                if INs[p] is None and prev is not None and p not in roots and \
                        (reach[prev][1] in UNCOND or noreturn(reach[prev][1], reach[prev][2])):
                    roots.append(p)
                    changed = True
                prev = p
        if roots and call_outs:
            lp = dict(call_outs[0])
            for o in call_outs[1:]:
                lp = {k: v for k, v in lp.items() if o.get(k) == v}
            for r in roots:
                cur = INs[r]
                new = lp if cur is None else {k: v for k, v in cur.items() if lp.get(k) == v}
                if new != cur:
                    INs[r] = new
                    changed = True
    changed = False
    while changed:
        for p in order:
            if INs[p] is None:
                continue
            out, _ = transfer(E, p, reach[p], INs[p], opts)
            for s in succ[p]:
                if INs[s] is None:
                    INs[s] = dict(out)
                    changed = True
                else:
                    new = {k: v for k, v in INs[s].items() if out.get(k) == v}
                    if new != INs[s]:
                        INs[s] = new
                        changed = True
    toks = []
    for p in order:
        if INs[p] is None:
            INs[p] = {}
        _, tok = transfer(E, p, reach[p], INs[p], opts)
        if tok is not None:
            toks.append((p, tok))
    return toks, fill, tables


def finalize(toks, tables, f):
    kept = []
    for p, t in toks:
        r = f([t])
        if r:
            kept.append((p, r[0]))
    idx = {}
    for i, (p, _) in enumerate(kept):
        idx.setdefault(p, i)
    sa = sorted(idx)

    def ti(t):
        j = bisect.bisect_left(sa, t)
        return idx[sa[j]] if j < len(sa) else len(kept)
    out = []
    for p, t in kept:
        m = re.search(r' ->([0-9a-f]+)$', t)
        if m:
            t = t[:m.start()] + ' ->L%d' % ti(int(m.group(1), 16))
        out.append(t)
    return out, [ti(t) for t in tables.values()]


def pref_loads(E, fn):
    a, end, reach, fill, tables = E.function(fn)
    n, strs = 0, []
    for p, (ln, op, args) in reach.items():
        if op == 'l32r':
            m = re.match(r'(a\d+), ([0-9a-f]+)', args)
            v = E.word(int(m.group(2), 16))
            lit = E.literal(v)
            if lit[0] == 'pref':
                n += 1
            if lit[0] == 'str':
                strs.append(v)
    return n, strs


# The registry constructor: its body is the planned change of 2a.3.3 (the 124 member constructions moved
# to the layer constructors; the push_back targets are layer fields).
PLANNED = {'UserPreferences::UserPreferences()'}


def COMDAT_CALLS(a, b):
    """A template instance kept from another translation unit: the calls may differ only in standard
    library helpers that one copy inlined (vector destructor vs operator delete)."""
    std = lambda c: {k: v for k, v in c.items() if not (k.startswith(('_ZNSt', '_ZdlPv', '_ZNKSt')))}
    return std(a) == std(b)


STEPS = [('base', lambda t: t, {}),
         ('alignment', lambda t: align_norm(t), {}),
         ('regalloc', lambda t: reg_norm(align_norm(t)), {}),
         ('scheduling', lambda t: reg_norm(align_norm(t)), {'blocks': True}),
         ('ref-indirection', lambda t: reg_norm(align_norm(t)), {'indirection': True, 'blocks': True})]


def blocks(fin):
    """(tokens, tables) -> per basic block sorted tokens: block boundaries at branch / jump / return
    tokens and at every branch or table target."""
    toks, tabs = fin
    targets = set(tabs)
    for t in toks:
        m = re.search(r' ->L(\d+)$', t)
        if m:
            targets.add(int(m.group(1)))
    out, cur = [], []
    for i, t in enumerate(toks):
        if i in targets and cur:
            out.append(sorted(cur)); cur = []
        cur.append(t)
        if ' ->L' in t or t.startswith(('ret', 'j ', 'jx ')):
            out.append(sorted(cur)); cur = []
    if cur:
        out.append(sorted(cur))
    return out, tabs


def effects(toks):
    """Multisets of a function's effects from its tokens (order ignored):
    preference accesses (r / w, field.member), preference addresses used (passed, stored, compared),
    calls (direct by name, indirect as callx), non-preference literals."""
    from collections import Counter
    acc, addr, calls, lits = Counter(), Counter(), Counter(), Counter()
    fnlit = {}            # register -> function symbol loaded by l32r (for callx after linker relaxation)
    for t in toks:
        m = re.match(r'^l32r (a\d+), (.+)\+0$', t)
        if m and m.group(2) in FUNCS:
            fnlit[m.group(1)] = m.group(2)
            continue
        m = re.match(r'^callx\d+ (a\d+)', t)
        if m and m.group(1) in fnlit:
            calls[FUNCS[fnlit.pop(m.group(1))]] += 1
            continue
        m = re.match(r'^(\S+) (&\S+|a\d+), \[([^\]]+)\]', t)
        if m:
            acc[('w' if m.group(1).startswith('s') else 'r', m.group(3))] += 1
            if m.group(2).startswith('&'):
                addr[m.group(2)[1:]] += 1
            continue
        for g in re.findall(r'\{([^}]*)\}', t):
            for x in g.split(','):
                addr[x.split('=&', 1)[1]] += 1
        m = re.match(r'^(call\d+) <([^>]*)>', t)
        if m:
            calls[m.group(2)] += 1
        elif t.startswith('callx'):
            calls['callx'] += 1
        if t.startswith('l32r '):
            lits[t.split(', ', 1)[1]] += 1
    # literals: each one must exist on the other side; how often it is loaded is not compared
    return acc, addr, calls, frozenset(lits)


FUNCS = {}    # demangled function name -> mangled (for call tokens), filled in main


def classify(R, C, fn):
    cache = {}

    def get(E, opts):
        k = (id(E), bool(opts.get('indirection')))
        if k not in cache:
            cache[k] = analyze(E, fn, opts)
        return cache[k]
    nr, sr = pref_loads(R, fn)
    nc, sc = pref_loads(C, fn)
    level, last = 'manual', None
    fr = fc = None
    for name, f, opts in STEPS:
        tr, fr, tabr = get(R, opts)
        tc, fc, tabc = get(C, opts)
        a = finalize(tr, tabr, f)
        b = finalize(tc, tabc, f)
        last = (a, b)
        if opts.get('blocks'):
            a, b = blocks(a), blocks(b)
        zero = set(fr) <= {0} and set(fc) <= {0}
        if a == b and zero and (name != 'base' or len(fr) == len(fc)):
            level = name
            break
    if level == 'manual':
        tr, _, _ = get(R, {'indirection': True})
        tc, _, _ = get(C, {'indirection': True})
        er, ec = effects([t for _, t in tr]), effects([t for _, t in tc])
        if er == ec:
            level = 'same-effects'
        elif fn in PLANNED:
            level = 'planned'
        elif R.syms[fn][0][2] == 'W' and fn.startswith('UserPref<') and er[0] == ec[0] and er[1] == ec[1] \
                and er[3] == ec[3] and COMDAT_CALLS(er[2], ec[2]):
            level = 'comdat'
        else:
            last = ((sorted(map(str, er[0].items())) + sorted(map(str, er[1].items())) + sorted(map(str, er[2].items()))
                     + sorted(map(str, er[3])), []),
                    (sorted(map(str, ec[0].items())) + sorted(map(str, ec[1].items())) + sorted(map(str, ec[2].items()))
                     + sorted(map(str, ec[3])), []))
    pats = []
    if fn.startswith('UserPreferences::'):
        pats.append('registry-offset')
    if nr != nc:
        pats.append('two-bases')
    if sr != sc:
        pats.append('rodata-moved')
    if level == 'base':
        level = pats[0] if pats else 'pref-addressing'
    return level, pats, last, (fr, fc)


def main(argv):
    ref_elf, cand_elf, ref_hdr, cand_dir = argv[1:5]
    R = Elf(ref_elf, ref_layout(ref_hdr))
    C = Elf(cand_elf, cand_layout(cand_dir))
    for E in (R, C):
        out = subprocess.run([TC + 'nm', E.path], capture_output=True, text=True).stdout
        dem = subprocess.run([TC + 'nm', '-C', E.path], capture_output=True, text=True).stdout
        for a, b in zip(out.splitlines(), dem.splitlines()):
            pa, pb = a.split(' ', 2), b.split(' ', 2)
            if len(pa) == 3 and len(pb) == 3 and pa[1] in 'tTwW':
                FUNCS[pb[2]] = pa[2]
    for o in PREF_OBJS:
        if o in R.layout.objs:
            assert R.layout.objs[o][1] == R.syms[o][0][1], (o, R.layout.objs[o][1], R.syms[o][0][1])
        assert C.layout.objs[o][1] == C.syms[o][0][1], (o, C.layout.objs[o][1], C.syms[o][0][1])
    if '--detail' in argv:
        import difflib
        fn = argv[argv.index('--detail') + 1]
        level, pats, (a, b), fill = classify(R, C, fn)
        print(level, pats, 'fill', fill)
        for l in difflib.unified_diff(a[0], b[0], 'ref', 'cand', lineterm='', n=2):
            print(l)
        print('tables', a[1], b[1])
        return 0
    changed = sorted(n for n, la in R.syms.items() if n in C.syms and la[0][2] in 'tTwW'
                     and C.syms[n][0][2] in 'tTwW' and la[0][1] != C.syms[n][0][1])
    cats = OrderedDict((k, []) for k in ['pref-addressing', 'two-bases', 'registry-offset', 'rodata-moved',
                                         'alignment', 'regalloc', 'scheduling', 'ref-indirection',
                                         'same-effects', 'comdat', 'planned', 'manual'])
    for n in changed:
        level, pats, _, fill = classify(R, C, n)
        cats[level].append((n, R.syms[n][0][1], C.syms[n][0][1], pats, len(fill[0]), len(fill[1])))
    print('size-changed functions present in both:', len(changed))
    for k, v in cats.items():
        print('\n## %s: %d' % (k, len(v)))
        for n, a, b, pats, f1, f2 in v:
            print('  %-62s %5d -> %5d  %s%s' % (n[:62], a, b, ','.join(pats),
                                                '  fill %d->%d' % (f1, f2) if f1 != f2 else ''))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
