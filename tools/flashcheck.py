"""flashcheck.py — base module of tools/flash_check.py (layouts of the preference objects, ELF access,
literal resolution, decoding helpers, align_norm / reg_norm). Use tools/flash_check.py; this file's own
command line (below, and `__main__`) is the first version of the check (linear tracking), superseded
by flash_check.py and kept only because flash_check.py imports this module unchanged.

Original header (v1):
2a.3.3 flash check: every function present in both ELFs whose size changed gets exactly one category.
Usage: python -I flashcheck.py <ref.elf> <cand.elf> <ref_UserPreferences.h> <cand_Preferences_dir> [--detail <fn>]

Decoding: recursive descent from the function entry (fallthrough, branch / jump / loop targets, and the
targets of jump tables: words in .flash.rodata that point into the function). Bytes of the function that
no reachable instruction covers are compared as data (fill bytes).
Each reachable instruction becomes a token:
  * l32r: the literal word is resolved. Preference objects (userPreferences and the layer objects) are
    tracked as base registers; any other address must resolve to the same symbol+offset, or to .rodata
    bytes with the same content (string moved); a plain constant must be equal.
  * base computations on a preference object (l32r, addmi, addi, mov) produce no token; a load or store
    through such a base becomes a token naming the preference field and the member inside UserPref
    (e.g. `l8ui aX, [colorUp._value]`); a preference address used any other way (call argument, store
    of the address, compare) becomes a token with the field address.
  * branch targets are replaced by the index of the target token.
Categories (the first level at which ref == cand after normalization):
  pref-addressing  only preference addressing differs, same number of base loads
  two-bases        as above, but the number of preference base loads differs
  registry-offset  as above, inside a UserPreferences member (offsets on `this`)
  rodata-moved     also a literal into .rodata that moved with identical content
  alignment        also narrow/wide forms, NOPs (or a1,a1,a1) and fill bytes
  regalloc         also a consistent renaming of registers
  ref-indirection  (NOT pre-approved) also the load of userContent.textPrefs, a reference to
                   userPreferences.textPrefs bound in UserContent::UserContent
  manual           anything else
"""
import sys, os, re, struct, bisect, subprocess
from collections import Counter, OrderedDict
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

class Elf:
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

    def function(self, name):
        a, s, _ = self.syms[name][0]
        end = a + s
        cache = {}
        decoded = {}

        def at(addr):
            if addr not in decoded:
                for k, v in self.decode_from(addr, end).items():
                    decoded.setdefault(k, v)
                if addr not in decoded:
                    raise RuntimeError('%s: cannot decode at 0x%x' % (name, addr))
            return decoded[addr]

        # jump table targets
        lo = bisect.bisect_left(self.ptr_keys, a)
        hi = bisect.bisect_left(self.ptr_keys, end)
        table_targets = self.ptr_keys[lo:hi]
        reach, work = {}, [a] + table_targets
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
        gaps = [x for x in range(a, end) if x not in covered]
        gap_bytes = self.read(a, s)
        fill = bytes(gap_bytes[x - a] for x in gaps)
        tables = OrderedDict()
        for t in table_targets:
            for w in self.rod_ptrs[t]:
                tables[w] = t
        return a, end, OrderedDict(sorted(reach.items())), fill, tables


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


# --------------------------------------------------------------------------- tokenizing

def tokens(E, fn, opts, flags):
    a, end, reach, fill, tables = E.function(fn)
    addrs = list(reach)
    base = {}             # reg -> (obj, off) for registers holding a preference address
    toks, tok_addr = [], []
    is_member = fn.startswith('UserPreferences::')
    if is_member:
        base['a2'] = ('userPreferences', 0)
    pref_loads = 0
    for p in addrs:
        ln, op, args = reach[p]
        ops = [x.strip() for x in args.split(',')] if args else []
        dst = ops[0] if ops else None
        tok = None
        lit = None
        if op == 'l32r':
            m = re.match(r'(a\d+), ([0-9a-f]+)', args)
            v = E.word(int(m.group(2), 16))
            lit = E.literal(v)
            if lit[0] == 'pref':
                base[m.group(1)] = (lit[1], lit[2])
                pref_loads += 1
                toks_append = None
            else:
                base.pop(m.group(1), None)
                if lit[0] == 'str':
                    tok = 'l32r %s, STR:%r' % (m.group(1), lit[1])
                    flags.setdefault('str', set()).add(v)
                elif lit[0] == 'sym':
                    tok = 'l32r %s, %s' % (m.group(1), lit[1])
                else:
                    tok = 'l32r %s, 0x%x' % (m.group(1), lit[1])
        elif op in ('addmi', 'addi', 'addi.n') and len(ops) == 3 and ops[1] in base:
            o, off = base[ops[1]]
            base[ops[0]] = (o, off + int(ops[2], 0))
            if op == 'addmi':
                flags['addmi'] = flags.get('addmi', 0) + 1
        elif op in ('mov.n', 'mov') and len(ops) == 2 and ops[1] in base:
            base[ops[0]] = base[ops[1]]
        elif op == 'or' and len(ops) == 3 and ops[1] == ops[2] and ops[1] in base:
            base[ops[0]] = base[ops[1]]
        elif re.match(r'^(l8ui|l16ui|l16si|l32i|l32i\.n|s8i|s16i|s32i|s32i\.n|lsi|ssi)$', op) and len(ops) == 3 \
                and ops[1] in base:
            o, off = base[ops[1]]
            eff = off + int(ops[2], 0)
            field = E.layout.resolve(o, eff)
            if op.startswith('s') and ops[0] in base:
                tok = '%s &%s, [%s]' % (op, E.layout.resolve(*base[ops[0]]), field)
            else:
                tok = '%s %s, [%s]' % (op, ops[0], field)
            if o == 'userContent' and field.startswith('textPrefs+0') and op.startswith('l32i'):
                # the reference userContent.textPrefs: its value is &userPreferences.textPrefs
                flags['indirection'] = flags.get('indirection', 0) + 1
                if opts.get('indirection'):
                    base[ops[0]] = ('@textPrefs', 0)
                    tok = None
                else:
                    base.pop(ops[0], None)
            elif not op.startswith('s'):
                base.pop(ops[0], None)
        else:
            # any other use of a preference address
            used = [r for r in ops if r in base]
            if used and not op.startswith('call'):
                desc = ','.join('%s=&%s' % (r, E.layout.resolve(*base[r])) for r in used)
                tok = '%s %s {%s}' % (op, args, desc)
            if op.startswith('call') or op.startswith('callx'):
                n = 4 if op.endswith('4') else 8 if op.endswith('8') else 12 if op.endswith('12') else 0
                if n:
                    args_used = ['%s=&%s' % (r, E.layout.resolve(*base[r])) for r in sorted(base)
                                 if int(r[1:]) >= n + 2 and int(r[1:]) <= 15]
                    if args_used:
                        toks.append('ARGS ' + ','.join(args_used))
                        tok_addr.append(p)
                    for r in list(base):
                        if int(r[1:]) >= n:
                            base.pop(r)
            if tok is None:
                t = target_of(op, args)
                a2 = re.sub(r'[0-9a-f]{8} <[^>]*>', 'T', args)
                tok = '%s %s' % (op, a2)
                if t is not None:
                    tok += ' ->%s' % ('%x' % t)
                elif op.startswith('call'):
                    m = re.search(r'<([^>+]*)', args)
                    tok = '%s <%s>' % (op, m.group(1) if m else args)
            if dst and re.match(r'^a\d+$', dst) and not NOWRITE.match(op):
                base.pop(dst, None)
        if base.get('@textPrefs'):
            pass
        if tok is not None:
            toks.append(tok)
            tok_addr.append(p)
    # resolve the textPrefs indirection: registers based on '@textPrefs' are userPreferences.textPrefs
    # (they were resolved through layout lookups of '@textPrefs', which do not exist) -> rewrite
    toks = [t.replace('@textPrefs?+', 'textPrefs+') for t in toks]
    # branch targets -> token index
    idx = {}
    for i, p in enumerate(tok_addr):
        idx.setdefault(p, i)
    sorted_addr = sorted(idx)

    def tindex(t):
        j = bisect.bisect_left(sorted_addr, t)
        return idx[sorted_addr[j]] if j < len(sorted_addr) else len(toks)
    out = []
    for t in toks:
        m = re.search(r' ->([0-9a-f]+)$', t)
        if m:
            t = t[:m.start()] + ' ->L%d' % tindex(int(m.group(1), 16))
        out.append(t)
    tabs = [tindex(t) for t in tables.values()]
    flags['pref_loads'] = pref_loads
    return out, fill, tabs


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


def drop_idx(toks):
    """after removing NOP tokens indices shift; recompute targets from the kept positions"""
    return toks


def compare(R, C, fn):
    fr, fc = {}, {}
    tr, fillr, tabr = tokens(R, fn, {}, fr)
    tc, fillc, tabc = tokens(C, fn, {}, fc)
    res = {'fill': (fillr, fillc), 'tables': (tabr, tabc), 'fr': fr, 'fc': fc}

    def eq(a, b):
        return a == b and tabr == tabc

    def strs_moved():
        return bool(fr.get('str')) and fr.get('str') != fc.get('str')

    level = None
    if eq(tr, tc) and fillr == fillc:
        level = 'pref'
    else:
        # alignment: renormalize with NOPs removed (indices recomputed by removing first, then indexing)
        ar, ac = norm_with(R, fn, align_norm, {}), norm_with(C, fn, align_norm, {})
        if ar[0] == ac[0] and ar[1] == ac[1] and set(fillr) <= {0} and set(fillc) <= {0}:
            level = 'alignment'
        else:
            rr = norm_with(R, fn, lambda t: reg_norm(align_norm(t)), {})
            rc = norm_with(C, fn, lambda t: reg_norm(align_norm(t)), {})
            if rr[0] == rc[0] and rr[1] == rc[1] and set(fillr) <= {0} and set(fillc) <= {0}:
                level = 'regalloc'
            else:
                ir = norm_with(R, fn, lambda t: reg_norm(align_norm(t)), {'indirection': True})
                ic = norm_with(C, fn, lambda t: reg_norm(align_norm(t)), {'indirection': True})
                if fc.get('indirection') and ir[0] == ic[0] and ir[1] == ic[1]:
                    level = 'ref-indirection'
                else:
                    level = 'manual'
                    res['diff'] = (rr[0], rc[0])
    if level == 'pref':
        if fn.startswith('UserPreferences::'):
            level = 'registry-offset'
        elif fr['pref_loads'] != fc['pref_loads']:
            level = 'two-bases'
        elif strs_moved():
            level = 'rodata-moved'
        else:
            level = 'pref-addressing'
    res['level'] = level
    res['patterns'] = [k for k, v in (('two-bases', fr['pref_loads'] != fc['pref_loads']),
                                      ('rodata-moved', strs_moved()),
                                      ('registry', fn.startswith('UserPreferences::')),
                                      ('fill', fillr != fillc)) if v]
    return res


def norm_with(E, fn, f, opts):
    """tokenize, apply f to the raw tokens with targets as addresses, then index."""
    flags = {}
    a, end, reach, fill, tables = E.function(fn)
    toks, _, _ = tokens_raw(E, fn, opts)
    keep = [(p, t2) for (p, t), t2 in zip(toks, f([t for _, t in toks])) if t2 is not None]
    # f may drop tokens: recompute by applying f per token
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


def tokens_raw(E, fn, opts):
    """tokens with their addresses and raw (address) branch targets"""
    global _raw
    flags = {}
    _raw = []
    saved = tokens.__code__
    out, fill, tabs = tokens_addr(E, fn, opts, flags)
    return out, fill, tabs


def tokens_addr(E, fn, opts, flags):
    # same as tokens(), but keeps (address, token) pairs with raw targets
    a, end, reach, fill, tables = E.function(fn)
    res = []
    orig_append = None
    toks, tok_addr = _tok_core(E, fn, opts, flags, reach)
    return list(zip(tok_addr, toks)), fill, tables


def _tok_core(E, fn, opts, flags, reach):
    # re-run the tokenizer loop and capture before target indexing
    import types
    src_toks = []
    # call tokens() internals by temporarily patching: simplest is to duplicate the loop
    base = {}
    toks, tok_addr = [], []
    if fn.startswith('UserPreferences::'):
        base['a2'] = ('userPreferences', 0)
    for p in reach:
        ln, op, args = reach[p]
        ops = [x.strip() for x in args.split(',')] if args else []
        dst = ops[0] if ops else None
        tok = None
        if op == 'l32r':
            m = re.match(r'(a\d+), ([0-9a-f]+)', args)
            v = E.word(int(m.group(2), 16))
            lit = E.literal(v)
            if lit[0] == 'pref':
                base[m.group(1)] = (lit[1], lit[2])
            else:
                base.pop(m.group(1), None)
                if lit[0] == 'str':
                    tok = 'l32r %s, STR:%r' % (m.group(1), lit[1])
                elif lit[0] == 'sym':
                    tok = 'l32r %s, %s' % (m.group(1), lit[1])
                else:
                    tok = 'l32r %s, 0x%x' % (m.group(1), lit[1])
        elif op in ('addmi', 'addi', 'addi.n') and len(ops) == 3 and ops[1] in base:
            o, off = base[ops[1]]
            base[ops[0]] = (o, off + int(ops[2], 0))
        elif op in ('mov.n', 'mov') and len(ops) == 2 and ops[1] in base:
            base[ops[0]] = base[ops[1]]
        elif op == 'or' and len(ops) == 3 and ops[1] == ops[2] and ops[1] in base:
            base[ops[0]] = base[ops[1]]
        elif re.match(r'^(l8ui|l16ui|l16si|l32i|l32i\.n|s8i|s16i|s32i|s32i\.n|lsi|ssi)$', op) and len(ops) == 3 \
                and ops[1] in base:
            o, off = base[ops[1]]
            eff = off + int(ops[2], 0)
            field = E.layout.resolve(o, eff) if o != '@textPrefs' else 'textPrefs+%d' % eff
            if op.startswith('s') and ops[0] in base:
                o2 = base[ops[0]]
                tok = '%s &%s, [%s]' % (op, E.layout.resolve(*o2) if o2[0] != '@textPrefs' else 'textPrefs+%d' % o2[1], field)
            else:
                tok = '%s %s, [%s]' % (op, ops[0], field)
            if o == 'userContent' and field == 'textPrefs+0' and op.startswith('l32i'):
                if opts.get('indirection'):
                    base[ops[0]] = ('@textPrefs', 0)
                    tok = None
                else:
                    base.pop(ops[0], None)
            elif not op.startswith('s'):
                base.pop(ops[0], None)
        else:
            if op.startswith('call'):
                n = 4 if op.endswith('4') else 8 if op.endswith('8') else 12 if op.endswith('12') else 0
                if n:
                    au = ['%s=&%s' % (r, E.layout.resolve(*base[r]) if base[r][0] != '@textPrefs'
                          else 'textPrefs+%d' % base[r][1]) for r in sorted(base) if n + 2 <= int(r[1:]) <= 15]
                    if au:
                        toks.append('ARGS ' + ','.join(au)); tok_addr.append(p)
                    for r in list(base):
                        if int(r[1:]) >= n:
                            base.pop(r)
            used = [r for r in ops if r in base]
            t = target_of(op, args)
            a2 = re.sub(r'[0-9a-f]{8} <[^>]*>', 'T', args)
            tok = '%s %s' % (op, a2)
            if used:
                tok += ' {%s}' % ','.join('%s=&%s' % (r, E.layout.resolve(*base[r]) if base[r][0] != '@textPrefs'
                                                      else 'textPrefs+%d' % base[r][1]) for r in used)
            if t is not None:
                tok += ' ->%x' % t
            elif op.startswith('call') and not op.startswith('callx'):
                m = re.search(r'<([^>+]*)', args)
                tok = '%s <%s>' % (op, m.group(1) if m else args)
            if dst and re.match(r'^a\d+$', dst) and not NOWRITE.match(op):
                base.pop(dst, None)
        if tok is not None:
            toks.append(tok)
            tok_addr.append(p)
    return toks, tok_addr


# --------------------------------------------------------------------------- main

def classify(R, C, fn):
    fr, fc = {}, {}

    def flags_of(E, f):
        a, end, reach, fill, tables = E.function(fn)
        nl = 0
        strs = []
        ind = 0
        for p, (ln, op, args) in reach.items():
            if op == 'l32r':
                m = re.match(r'(a\d+), ([0-9a-f]+)', args)
                lit = E.literal(E.word(int(m.group(2), 16)))
                if lit[0] == 'pref':
                    nl += 1
                if lit[0] == 'str':
                    strs.append(E.word(int(m.group(2), 16)))
        f['pref_loads'] = nl
        f['strs'] = strs
        f['fill'] = fill
        return f
    flags_of(R, fr)
    flags_of(C, fc)
    zero_fill = set(fr['fill']) <= {0} and set(fc['fill']) <= {0}
    steps = [('base', lambda t: t, {}),
             ('alignment', align_norm, {}),
             ('regalloc', lambda t: reg_norm(align_norm(t)), {}),
             ('ref-indirection', lambda t: reg_norm(align_norm(t)), {'indirection': True})]
    level, last = 'manual', None
    for name, f, opts in steps:
        a = norm_with(R, fn, f, opts)
        b = norm_with(C, fn, f, opts)
        last = (a, b)
        if a == b and (name != 'base' or fr['fill'] == fc['fill']) and zero_fill:
            level = name
            break
    pats = []
    if fn.startswith('UserPreferences::'):
        pats.append('registry-offset')
    if fr['pref_loads'] != fc['pref_loads']:
        pats.append('two-bases')
    if fr['strs'] != fc['strs']:
        pats.append('rodata-moved')
    if level == 'base':
        level = pats[0] if pats else 'pref-addressing'
    return level, pats, last, (fr['fill'], fc['fill'])


if __name__ == '__main__':
    ref_elf, cand_elf, ref_hdr, cand_dir = sys.argv[1:5]
    R = Elf(ref_elf, ref_layout(ref_hdr))
    C = Elf(cand_elf, cand_layout(cand_dir))
    for o in PREF_OBJS:
        if o in R.layout.objs:
            assert R.layout.objs[o][1] == R.syms[o][0][1], (o, R.layout.objs[o][1], R.syms[o][0][1])
        assert C.layout.objs[o][1] == C.syms[o][0][1], (o, C.layout.objs[o][1], C.syms[o][0][1])
    if '--detail' in sys.argv:
        fn = sys.argv[sys.argv.index('--detail') + 1]
        level, pats, (a, b), fill = classify(R, C, fn)
        import difflib
        print(level, pats, 'fill', fill)
        for l in difflib.unified_diff(a[0], b[0], 'ref', 'cand', lineterm='', n=2):
            print(l)
        print('tables', a[1], b[1])
        sys.exit(0)
    changed = []
    for n, la in R.syms.items():
        lb = C.syms.get(n)
        if lb and la[0][2] in 'tTwW' and lb[0][2] in 'tTwW' and la[0][1] != lb[0][1]:
            changed.append(n)
    cats = OrderedDict()
    for n in sorted(changed):
        level, pats, _, fill = classify(R, C, n)
        cats.setdefault(level, []).append((n, R.syms[n][0][1], C.syms[n][0][1], pats, len(fill[0]), len(fill[1])))
    print('size-changed functions present in both:', len(changed))
    for k, v in cats.items():
        print('\n## %s: %d' % (k, len(v)))
        for n, a, b, pats, f1, f2 in v:
            print('  %-60s %5d -> %5d  %s%s' % (n[:60], a, b, ','.join(pats), '  fill %d->%d' % (f1, f2) if f1 != f2 else ''))
