r"""
prefs_write_test.py — tests the preference write entry point of each model on the PC (stage 2b.1).

Compiles a model's real registry code (<src>/Preferences/*.cpp, as tools/prefs_contract.py does) with
tools/prefs_write_test/main.cpp, against the host stubs of tools/prefs_contract/stub, runs it, and
checks every write case. The firmware sources are only read, never changed. Needs the 2b.1 registry
(UserPreferences::write / writeBytes, UserPref<T>::decode); does not compile against older trees.

What is tested (main.cpp, one line per case: PASS / FAIL, result, value after the write, NVS puts):
    * writing the value already in RAM (every default, through toBytes -> writeBytes) returns
      UNCHANGED and saves nothing (owner's decision 2 of the 2b plan);
    * the six registered checks at their boundaries: shita < numOfShitot, ofek < numOfOfeks, the four
      zeet keys <= numOfZeetCochavin with 0 accepted (P8); the typed entry write<T> as well;
    * a preference without a check accepts any value of its type (as before 2b.1, P1);
    * short lengths per type (BAD_BYTES), negative I8 / I16, double, String with and without \0, a
      String cut at \0, a 70-byte String, a text slot;
    * a change saves exactly once, with the new value; a rejected or bad write leaves the value and NVS.

The checks themselves live in DB/Place.cpp and DB/Shita.cpp, which cannot be compiled on the host.
main.cpp has copies; this script first verifies in the model's source that the bodies and the
registrations are exactly those copies, that main.cpp calls registerPlaceChecks() and
registerShitaChecks() before initBluetooth(), and passes the model's bounds to the build:
numOfOfeks = the number of entries in places[] (DB/Place.cpp; initPlace sets it the same way),
numOfShitot and numOfZeetCochavin from DB/Shita.h.

Usage (Python 3.8+, no packages needed; run from the repo root, Git Bash):

    python -I tools/prefs_write_test.py [<model> ...] [--cxx auto|msvc|g++]

    <model>   JC6416, JC4020 or JC6432 (all three if none given)
    --cxx     as in prefs_contract.py

Exit code 1 if any check or case fails, 2 on a tool error. Builds in a temporary folder outside the
repo. Writes nothing in the repo. No bytecode is written.
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import prefs_contract as pc  # noqa: E402  (same folder; ToolError, source_files, find_vcvars)

REPO = pc.REPO
TOOL_DIR = os.path.join(HERE, 'prefs_write_test')
STUB_DIR = pc.STUB_DIR
FORMAT_DIR = pc.TOOL_DIR
ToolError = pc.ToolError


def read(path):
    with open(path, encoding='utf-8', errors='replace') as f:
        return f.read()


def ws(s):
    """Whitespace-insensitive regex for a code fragment."""
    return r'\s*'.join(re.escape(t) for t in s.split())


def model_bounds(src):
    """numOfOfeks, numOfShitot, numOfZeetCochavin of the model, from its sources."""
    place = read(os.path.join(src, 'DB', 'Place.cpp'))
    m = re.search(r'^Place places\[\] = \{(.*?)^\};', place, re.S | re.M)
    if not m:
        raise ToolError('places[] not found in DB/Place.cpp')
    body = re.sub(r'//[^\n]*', '', m.group(1))
    ofeks = len(re.findall(r'\bPlace\s*\(', body))
    shita_h = read(os.path.join(src, 'DB', 'Shita.h'))
    vals = {}
    for name in ('numOfShitot', 'numOfZeetCochavin'):
        m = re.search(r'const int %s = (\d+);' % name, shita_h)
        if not m:
            raise ToolError('%s not found in DB/Shita.h' % name)
        vals[name] = int(m.group(1))
    return ofeks, vals['numOfShitot'], vals['numOfZeetCochavin']


def source_checks(src):
    """The registered checks in the model's source equal the copies in main.cpp. Returns error lines."""
    place = read(os.path.join(src, 'DB', 'Place.cpp'))
    shita = read(os.path.join(src, 'DB', 'Shita.cpp'))
    main = read(os.path.join(src, 'main.cpp'))
    want = [
        ('DB/Place.cpp', place, 'static bool ofekInRange(const uint8_t &value) { return value < numOfOfeks; }'),
        ('DB/Place.cpp', place, 'void registerPlaceChecks() { '
                                'UserPreferences::registerCheck(dataPrefs.place.ofek, ofekInRange); }'),
        ('DB/Shita.cpp', shita, 'static bool shitaInRange(const uint8_t &value) { return value < numOfShitot; }'),
        ('DB/Shita.cpp', shita, 'static bool zeetInRange(const uint8_t &value) '
                                '{ return value <= numOfZeetCochavin; }'),
        ('DB/Shita.cpp', shita, 'void registerShitaChecks() { '
                                'UserPreferences::registerCheck(dataPrefs.zmanim.shita, shitaInRange); '
                                'UserPreferences::registerCheck(dataPrefs.zmanim.zeetCochavim, zeetInRange); '
                                'UserPreferences::registerCheck(dataPrefs.zmanim.rabenuTam, zeetInRange); '
                                'UserPreferences::registerCheck(dataPrefs.zmanim.zeetShabat, zeetInRange); '
                                'UserPreferences::registerCheck(dataPrefs.zmanim.zeetZom, zeetInRange); }'),
        ('main.cpp', main, 'registerPlaceChecks(); registerShitaChecks(); '
                           'if(systemPrefs.bt == 1) initBluetooth();'),
    ]
    errors = []
    for name, text, code in want:
        n = len(re.findall(ws(code), text))
        if n != 1:
            errors.append('%s: expected once, found %d: %s' % (name, n, code))
    # nothing else registers a check, and only setup calls the registration functions
    for root, _, files in os.walk(src):
        for fn in files:
            if not fn.endswith(('.cpp', '.h')):
                continue
            path = os.path.join(root, fn)
            rel = os.path.relpath(path, src).replace('\\', '/')
            text = re.sub(r'//[^\n]*', '', read(path))
            if rel not in ('DB/Place.cpp', 'DB/Shita.cpp', 'Preferences/UserPreferences.h') and \
                    re.search(r'\bregisterCheck\s*\(', text):
                errors.append('%s: unexpected registerCheck call' % rel)
            calls = len(re.findall(r'\bregister(Place|Shita)Checks\s*\(\s*\)\s*;', text))
            if rel == 'main.cpp' and calls != 2:
                errors.append('main.cpp: expected 2 registration calls, found %d' % calls)
    return errors


def compile_and_run(src, cxx, defines):
    pdir, files = pc.source_files(src)
    sources = files + [os.path.join(TOOL_DIR, 'main.cpp')]
    if cxx == 'auto':
        cxx = 'g++' if shutil.which('g++') else 'msvc'
    with tempfile.TemporaryDirectory(prefix='prefs_write_test_') as tmp:
        exe = os.path.join(tmp, 'write_test.exe')
        if cxx == 'g++':
            cmd = (['g++', '-std=gnu++17', '-I', STUB_DIR, '-I', FORMAT_DIR, '-I', pdir] +
                   ['-D%s=%d' % kv for kv in defines.items()] + sources + ['-o', exe])
            res = subprocess.run(cmd, capture_output=True, text=True, errors='replace')
        elif cxx == 'msvc':
            bat = os.path.join(tmp, 'build.bat')
            line = 'cl /nologo /std:c++17 /EHsc /GR /utf-8 /I "%s" /I "%s" /I "%s" %s %s /Fo"%s\\\\" /Fe"%s"' % (
                STUB_DIR, FORMAT_DIR, pdir, ' '.join('/D%s=%d' % kv for kv in defines.items()),
                ' '.join('"%s"' % s for s in sources), tmp, exe)
            with open(bat, 'w', newline='\r\n') as f:
                f.write('@echo off\n')
                f.write('call "%s" >nul\n' % pc.find_vcvars())
                f.write(line + '\n')
            res = subprocess.run(['cmd', '/d', '/c', bat], capture_output=True, text=True,
                                 errors='replace', cwd=tmp)
        else:
            raise ToolError('unknown --cxx %s' % cxx)
        if res.returncode != 0 or not os.path.exists(exe):
            raise ToolError('compile failed (%s):\n%s%s' % (cxx, res.stdout, res.stderr))
        run = subprocess.run([exe], capture_output=True, cwd=tmp)
        return run.returncode, run.stdout.decode('utf-8', 'replace').replace('\r\n', '\n'), \
            run.stderr.decode('utf-8', 'replace')


def test_model(model, cxx):
    src = os.path.join(REPO, model, 'src')
    ofeks, shitot, zeet = model_bounds(src)
    print('== %s  (numOfOfeks %d, numOfShitot %d, numOfZeetCochavin %d)' % (model, ofeks, shitot, zeet))
    errors = source_checks(src)
    for e in errors:
        print('FAIL\tsource\t%s' % e)
    if not errors:
        print('PASS\tsource: check bodies, registrations and the calls in setup as in main.cpp of the tool')
    code, out, err = compile_and_run(src, cxx, {'TEST_NUM_OFEKS': ofeks, 'TEST_NUM_SHITOT': shitot,
                                                'TEST_NUM_ZEET': zeet})
    sys.stdout.write(out)
    if err:
        sys.stdout.write(err)
    return not errors and code == 0


def main(argv):
    args = argv[1:]
    if args and args[0] in ('-h', '--help'):
        print(__doc__)
        return 0
    cxx = 'auto'
    if '--cxx' in args:
        i = args.index('--cxx')
        cxx = args[i + 1]
        del args[i:i + 2]
    models = args or pc.MODELS
    for m in models:
        if m not in pc.MODELS:
            raise ToolError('unknown model %s' % m)
    bad = [m for m in models if not test_model(m, cxx)]
    print('== %s' % ('FAILED: ' + ', '.join(bad) if bad else 'all passed: ' + ', '.join(models)))
    return 1 if bad else 0


if __name__ == '__main__':
    try:
        sys.exit(main(sys.argv))
    except ToolError as e:
        print('error: %s' % e, file=sys.stderr)
        sys.exit(2)
