"""Compare two ESP32 app images (firmware .bin) and explain where they differ.

Usage:
    python tools/compare_bins.py <reference.bin> <candidate.bin>

Differences are labeled by region:
    app_desc  - esp_app_desc_t (version, project name, time/date, ELF SHA-256)
    trailer   - image checksum and appended SHA-256 at the end of the image
    code/data - everything else: a real difference in compiled content
"""
import hashlib
import struct
import sys

APP_DESC_OFFSET = 0x20
APP_DESC_SIZE = 256
APP_DESC_MAGIC = 0xABCD5432
TRAILER_SIZE = 48


def read(path):
    with open(path, "rb") as f:
        return f.read()


def cstr(raw):
    return raw.split(b"\0", 1)[0].decode("ascii", errors="replace")


def app_desc(data):
    d = data[APP_DESC_OFFSET:APP_DESC_OFFSET + APP_DESC_SIZE]
    if len(d) < APP_DESC_SIZE or struct.unpack_from("<I", d, 0)[0] != APP_DESC_MAGIC:
        return None
    return {
        "version": cstr(d[16:48]),
        "project_name": cstr(d[48:80]),
        "time": cstr(d[80:96]),
        "date": cstr(d[96:112]),
        "idf_ver": cstr(d[112:144]),
        "elf_sha256": d[144:176].hex()[:16] + "...",
    }


def region(offset, length):
    if APP_DESC_OFFSET <= offset < APP_DESC_OFFSET + APP_DESC_SIZE:
        return "app_desc"
    if offset >= length - TRAILER_SIZE:
        return "trailer"
    return "code/data"


def diff_ranges(a, b):
    n = min(len(a), len(b))
    ranges, start = [], None
    for i in range(n):
        if a[i] != b[i]:
            if start is None:
                start = i
        elif start is not None:
            ranges.append((start, i))
            start = None
    if start is not None:
        ranges.append((start, n))
    return ranges


def main():
    if len(sys.argv) != 3:
        print(__doc__)
        sys.exit(2)
    ref_path, cand_path = sys.argv[1], sys.argv[2]
    a, b = read(ref_path), read(cand_path)

    print(f"reference: {ref_path}")
    print(f"  size {len(a):,}  md5 {hashlib.md5(a).hexdigest()}")
    print(f"candidate: {cand_path}")
    print(f"  size {len(b):,}  md5 {hashlib.md5(b).hexdigest()}")

    da, db = app_desc(a), app_desc(b)
    print("\napp_desc (reference | candidate):")
    if da and db:
        for key in da:
            mark = "" if da[key] == db[key] else "   <-- differs"
            print(f"  {key:13} {da[key]} | {db[key]}{mark}")
    else:
        print("  could not read esp_app_desc_t from one of the files")

    if a == b:
        print("\nRESULT: identical")
        return

    if len(a) != len(b):
        first = next((i for i in range(min(len(a), len(b))) if a[i] != b[i]), min(len(a), len(b)))
        print(f"\nRESULT: sizes differ by {len(b) - len(a):+,} bytes; first difference at 0x{first:X}")
        print("  Content differs (code/data). Most likely causes: different library versions,")
        print("  or different source paths embedded via __FILE__ (build folder moved).")
        return

    ranges = diff_ranges(a, b)
    by_region = {}
    for s, e in ranges:
        by_region.setdefault(region(s, len(a)), []).append((s, e))
    total = sum(e - s for s, e in ranges)
    print(f"\nRESULT: same size, {total:,} bytes differ in {len(ranges)} range(s)")
    for name, rs in by_region.items():
        print(f"  {name}: {len(rs)} range(s), {sum(e - s for s, e in rs):,} bytes")
        for s, e in rs[:5]:
            print(f"    0x{s:06X}-0x{e:06X}")
        if len(rs) > 5:
            print(f"    ... {len(rs) - 5} more")
    if "code/data" not in by_region:
        print("\n  Only metadata differs: compiled content is identical.")


if __name__ == "__main__":
    main()