r"""
prefs_contract.py — the preference registry of each model as the code builds it. Contract baseline.

Compiles a model's real registry code on the PC, against minimal host stubs, runs it, and prints
what it built. The firmware sources are only read, never changed. Stage 2a.3 changes the registry
code and its binary, so this output (not the image) is the reference for the preference contract:
NVS keys, value types and namespace, BLE indexes and type codes, defaults, list order and load order.

What is compiled:
    <src>/Preferences/*.cpp, except PrefsViews.cpp (compile-time checks only), plus
    tools/prefs_contract/main.cpp. Include path: tools/prefs_contract/stub (String, Serial,
    Preferences, an empty States/States.h) and <src>/Preferences.
    RULE TO MAINTAIN: when the preference files move (layer folders, stage 1, or the per-layer
    owner files of 2a.3.3), update source_files() below so that every .cpp that defines a
    preference object or the registry is compiled, and nothing that needs the rest of the firmware.

What is printed (deterministic; one line per entry, tab-separated, in list order):
    [simplePrefs] N / [textPrefs] N / [allPrefs] N
        key  type-tag(code)  index  default  C++-type  namespace
        default: integers in decimal; double as %.15g plus the IEEE 754 bits; strings quoted.
    [nvs-load] N   every Preferences call made by setupPreferences(), in order:
        namespace  function  key  default-passed       (begin: namespace  begin  -  rw|ro)
    [serial] N     every line printed to Serial during setupPreferences() (expected: none)

Checks that make the tool fail: an entry whose UserPref<T> type is unknown, and any key or namespace
name longer than 15 characters (NVS limit: NVS_KEY_NAME_MAX_SIZE is 16 including the terminator),
in the lists or in the NVS log.

Baselines: docs/contracts/prefs/<model>.txt (LF). They are written only by `write` (CLAUDE.md,
exception to the editor-tools rule). `check` reads a CRLF working copy (core.autocrlf, after a
checkout) as LF.

Usage (Python 3.8+, no packages needed; run from the repo root, Git Bash):

    python -I tools/prefs_contract.py print <model> [--src <dir>]   # print to stdout
    python -I tools/prefs_contract.py check [<model> ...]           # compare with the baselines
    python -I tools/prefs_contract.py write [<model> ...]           # (re)write the baselines

    <model>      JC6416, JC4020 or JC6432 (check / write: all three if none given)
    --src <dir>  source tree to use instead of <model>/src (print only), e.g. a scratch copy
    --cxx auto|msvc|g++
                 auto (default): g++ if it is on PATH, else MSVC found with vswhere (vcvars64.bat)

check   exit code 1 and a unified diff for every model whose output differs from its baseline.

Builds in a temporary folder outside the repo. Writes nothing in the repo except in `write` mode.
No bytecode is written.
"""
import difflib
import glob
import os
import shutil
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
sys.stdout.reconfigure(encoding='utf-8', newline='\n')

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
TOOL_DIR = os.path.join(HERE, 'prefs_contract')
STUB_DIR = os.path.join(TOOL_DIR, 'stub')
BASE_DIR = os.path.join(REPO, 'docs', 'contracts', 'prefs')
MODELS = ['JC6416', 'JC4020', 'JC6432']
NVS_NAME_MAX = 15
VSWHERE = r'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'


class ToolError(Exception):
    pass


def source_files(src):
    pdir = os.path.join(src, 'Preferences')
    files = sorted(f for f in glob.glob(os.path.join(pdir, '*.cpp'))
                   if os.path.basename(f) != 'PrefsViews.cpp')
    if not files:
        raise ToolError('no .cpp files in %s' % pdir)
    return pdir, files


def find_vcvars():
    if not os.path.exists(VSWHERE):
        raise ToolError('MSVC not found (no vswhere.exe), and no g++ on PATH')
    path = subprocess.run([VSWHERE, '-latest', '-products', '*', '-requires',
                           'Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
                           '-property', 'installationPath'],
                          capture_output=True, text=True).stdout.strip()
    vcvars = os.path.join(path, 'VC', 'Auxiliary', 'Build', 'vcvars64.bat')
    if not path or not os.path.exists(vcvars):
        raise ToolError('MSVC C++ tools not found (vcvars64.bat)')
    return vcvars


def compile_and_run(src, cxx):
    pdir, files = source_files(src)
    sources = files + [os.path.join(TOOL_DIR, 'main.cpp')]
    if cxx == 'auto':
        cxx = 'g++' if shutil.which('g++') else 'msvc'
    with tempfile.TemporaryDirectory(prefix='prefs_contract_') as tmp:
        exe = os.path.join(tmp, 'contract.exe')
        if cxx == 'g++':
            cmd = ['g++', '-std=gnu++17', '-I', STUB_DIR, '-I', pdir] + sources + ['-o', exe]
            res = subprocess.run(cmd, capture_output=True, text=True, errors='replace')
        elif cxx == 'msvc':
            bat = os.path.join(tmp, 'build.bat')
            line = 'cl /nologo /std:c++17 /EHsc /GR /utf-8 /I "%s" /I "%s" %s /Fo"%s\\\\" /Fe"%s"' % (
                STUB_DIR, pdir, ' '.join('"%s"' % s for s in sources), tmp, exe)
            with open(bat, 'w', newline='\r\n') as f:
                f.write('@echo off\n')
                f.write('call "%s" >nul\n' % find_vcvars())
                f.write(line + '\n')
            res = subprocess.run(['cmd', '/d', '/c', bat], capture_output=True, text=True,
                                 errors='replace', cwd=tmp)
        else:
            raise ToolError('unknown --cxx %s' % cxx)
        if res.returncode != 0 or not os.path.exists(exe):
            raise ToolError('compile failed (%s):\n%s%s' % (cxx, res.stdout, res.stderr))
        run = subprocess.run([exe], capture_output=True, cwd=tmp)
        if run.returncode != 0:
            raise ToolError('contract program failed:\n%s' %
                            run.stderr.decode('utf-8', 'replace'))
        return run.stdout.decode('utf-8').replace('\r\n', '\n')


def check_nvs_names(body):
    """Fail on any key or namespace name longer than the NVS limit."""
    errors = []
    section = None
    for line in body.splitlines():
        if line.startswith('['):
            section = line.split(']')[0][1:]
            continue
        f = line.split('\t')
        if section in ('simplePrefs', 'textPrefs', 'allPrefs'):
            names = [('key', f[0]), ('namespace', f[5])]
        elif section == 'nvs-load':
            names = [('namespace', f[0]), ('key', f[2])]
        else:
            continue
        for what, name in names:
            if len(name.encode('utf-8')) > NVS_NAME_MAX:
                errors.append('[%s] %s "%s" is %d characters (NVS limit %d)'
                              % (section, what, name, len(name.encode('utf-8')), NVS_NAME_MAX))
    if errors:
        raise ToolError('\n'.join(errors))


def contract(model, src=None, cxx='auto'):
    src = src or os.path.join(REPO, model, 'src')
    body = compile_and_run(src, cxx)
    check_nvs_names(body)
    header = ('# prefs contract: %s (tools/prefs_contract.py)\n'
              '# columns: key, type tag (code), index, default, C++ type, namespace\n' % model)
    return header + body


def base_path(model):
    return os.path.join(BASE_DIR, model + '.txt')


def main(argv):
    args = argv[1:]
    if not args or args[0] in ('-h', '--help'):
        print(__doc__)
        return 0
    cmd = args.pop(0)
    src, cxx = None, 'auto'
    if '--src' in args:
        i = args.index('--src')
        src = args[i + 1]
        del args[i:i + 2]
    if '--cxx' in args:
        i = args.index('--cxx')
        cxx = args[i + 1]
        del args[i:i + 2]
    models = args or (MODELS if cmd in ('check', 'write') else [])
    for m in models:
        if m not in MODELS:
            raise ToolError('unknown model %s' % m)
    if cmd == 'print':
        if len(models) != 1:
            raise ToolError('print takes exactly one model')
        sys.stdout.write(contract(models[0], src, cxx))
        return 0
    if src:
        raise ToolError('--src is for print only')
    if cmd == 'write':
        os.makedirs(BASE_DIR, exist_ok=True)
        for m in models:
            with open(base_path(m), 'w', encoding='utf-8', newline='\n') as f:
                f.write(contract(m, None, cxx))
            print('written %s' % os.path.relpath(base_path(m), REPO).replace('\\', '/'))
        return 0
    if cmd == 'check':
        bad = 0
        for m in models:
            new = contract(m, None, cxx)
            try:
                with open(base_path(m), encoding='utf-8', newline='') as f:
                    old = f.read().replace('\r\n', '\n')   # CRLF after a checkout (autocrlf)
            except FileNotFoundError:
                old = ''
            if old == new:
                print('%s: OK' % m)
                continue
            bad += 1
            print('%s: DIFFERS' % m)
            sys.stdout.writelines(difflib.unified_diff(
                old.splitlines(True), new.splitlines(True),
                'baseline/%s.txt' % m, 'current/%s' % m))
        return 1 if bad else 0
    raise ToolError('unknown command %s' % cmd)


if __name__ == '__main__':
    try:
        sys.exit(main(sys.argv))
    except ToolError as e:
        print('error: %s' % e, file=sys.stderr)
        sys.exit(2)
