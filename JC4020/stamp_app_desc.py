#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
stamp_app_desc.py — הטבעת דגם וגרסה בתוך esp_app_desc_t של הבינארי

הבעיה: בבנייה של Arduino, המבנה esp_app_desc_t מגיע מהספריות המוכנות
מראש ולכן מכיל "arduino-lib-builder" ואת גרסת ה-IDF — לא את הדגם ולא
את הגרסה שלנו. המבנה יושב בכתובת קבועה (0x20 מתחילת ה-image), ולכן
הוא המקום היחיד שאפשר לקרוא ממנו בוודאות בלי לסרוק מגהבייטים.

הפתרון: אחרי הבנייה כותבים לשם את הערכים האמיתיים, ומחשבים מחדש את
ה-SHA-256 המצורף. מאותו רגע קריאה של 256 בייט מ-0x10000 מזהה בוודאות
דגם, גרסה, וחומרה — גם דרך הכבל, גם בכלי תמיכה, וגם בזמן ריצה דרך
esp_ota_get_app_description().

התקנה — ב-platformio.ini של כל דגם:

    extra_scripts = post:stamp_app_desc.py

הסקריפט קורא את הערכים מדגלי הבנייה (PROJECT_VER, DEVICE_MODEL,
HARDWARE_VERSION), ולכן platformio.ini נשאר מקור האמת היחיד.

פריסת esp_app_desc_t, יחסית לתחילת ה-image:
    0x20  magic_word    (0xABCD5432)
    0x24  secure_version
    0x30  version[32]        ← נכתב
    0x50  project_name[32]   ← נכתב
    0x70  time[16]
    0x80  date[16]
    0x90  idf_ver[32]
"""

import hashlib
import struct
import sys
from pathlib import Path

APP_DESC_OFF = 0x20
APP_DESC_MAGIC = 0xABCD5432
VERSION_OFF = APP_DESC_OFF + 0x10
PROJECT_OFF = APP_DESC_OFF + 0x30
FIELD_LEN = 32
SEG_COUNT_OFF = 1
HASH_APPENDED_OFF = 23
HEADER_LEN = 24
CHECKSUM_SEED = 0xEF


def _segments(data):
    """
    מחזיר (טווחי הנתונים של הסגמנטים, אופסט בית ה-checksum).

    מבנה image של ESP32:
        24 בייט header
        לכל סגמנט: 8 בייט כותרת (load_addr, data_len) ואחריהם הנתונים
        ריפוד עד שהאורך הכולל +1 מתחלק ב-16
        בית checksum אחד: XOR של כל נתוני הסגמנטים, עם seed 0xEF
        אם hash_appended: 32 בייט SHA-256 של כל מה שלפניו
    """
    pos, ranges = HEADER_LEN, []
    for _ in range(data[SEG_COUNT_OFF]):
        _addr, length = struct.unpack("<II", data[pos:pos + 8])
        pos += 8
        ranges.append((pos, pos + length))
        pos += length
    return ranges, pos + ((15 - (pos % 16)) % 16)


def _reseal(data):
    """
    חישוב מחדש של שתי החתימות אחרי שינוי בנתוני הסגמנטים.

    ה-app_desc יושב בתוך נתוני הסגמנט הראשון, ולכן כתיבה אליו מבטלת
    את ה-XOR checksum. בלי חישוב מחדש esp_image_verify נכשל — השעון
    לא יעלה, ועדכון OTA יידחה ב-esp_ota_end().
    """
    ranges, chk_off = _segments(data)
    if chk_off >= len(data):
        raise ValueError("מבנה ה-image לא תקין — בית ה-checksum מחוץ לקובץ")

    checksum = CHECKSUM_SEED
    for start, end in ranges:
        for byte in data[start:end]:
            checksum ^= byte
    data[chk_off] = checksum

    if data[HASH_APPENDED_OFF] == 1:
        data[-32:] = hashlib.sha256(bytes(data[:-32])).digest()


def _validate(data):
    """אימות עצמאי: XOR checksum ו-SHA-256. זורק חריגה אם משהו לא תקין."""
    ranges, chk_off = _segments(data)
    checksum = CHECKSUM_SEED
    for start, end in ranges:
        for byte in data[start:end]:
            checksum ^= byte
    if data[chk_off] != checksum:
        raise ValueError(
            f"ה-XOR checksum אינו תקין (בקובץ 0x{data[chk_off]:02x}, "
            f"חושב 0x{checksum:02x})")
    if data[HASH_APPENDED_OFF] == 1:
        if hashlib.sha256(bytes(data[:-32])).digest() != bytes(data[-32:]):
            raise ValueError("ה-SHA-256 המצורף אינו תקין")


def _fit(name, value):
    """מחרוזת מסתיימת ב-NUL בשדה קבוע. חורגת → עצירה, לא חיתוך שקט."""
    raw = value.encode("utf-8")
    if len(raw) >= FIELD_LEN:
        raise ValueError(
            f"{name} ארוך מדי ({len(raw)} בייט). המגבלה היא {FIELD_LEN - 1}.")
    return raw.ljust(FIELD_LEN, b"\x00")


def stamp(path, version, project):
    """
    מחזיר (before_version, before_project). זורק חריגה אם משהו לא תקין —
    עדיף שהבילד ייכשל מאשר שייצא bin שמזהה את עצמו כ-arduino-lib-builder.
    """
    data = bytearray(Path(path).read_bytes())

    if len(data) < 0x200 or data[0] != 0xE9:
        raise ValueError(f"{path} אינו image תקין של ESP32 (הבית הראשון אינו 0xE9)")

    magic = struct.unpack("<I", data[APP_DESC_OFF:APP_DESC_OFF + 4])[0]
    if magic != APP_DESC_MAGIC:
        raise ValueError(
            f"לא נמצא esp_app_desc_t בכתובת 0x{APP_DESC_OFF:02x} "
            f"(magic 0x{magic:08x} במקום 0x{APP_DESC_MAGIC:08x})")

    def read(off):
        return data[off:off + FIELD_LEN].split(b"\x00")[0].decode("utf-8", "replace")

    before = (read(VERSION_OFF), read(PROJECT_OFF))

    # אימות המקור לפני שנוגעים בו — אם הוא כבר פגום, עדיף לדעת עכשיו
    _validate(data)

    data[VERSION_OFF:VERSION_OFF + FIELD_LEN] = _fit("PROJECT_VER", version)
    data[PROJECT_OFF:PROJECT_OFF + FIELD_LEN] = _fit("DEVICE_MODEL", project)

    _reseal(data)

    Path(path).write_bytes(bytes(data))
    return before


def verify(path, version, project):
    """קריאה חוזרת מהקובץ שנכתב — אימות שההטבעה אכן תפסה."""
    data = Path(path).read_bytes()

    def read(off):
        return data[off:off + FIELD_LEN].split(b"\x00")[0].decode("utf-8", "replace")

    got = (read(VERSION_OFF), read(PROJECT_OFF))
    if got != (version, project):
        raise ValueError(f"האימות נכשל: בקובץ {got}, ציפינו ל-{(version, project)}")

    _validate(bytearray(data))
    return got


# ═══════════════════════════════════════════════════════════════
# שילוב ב-PlatformIO
# ═══════════════════════════════════════════════════════════════

try:
    Import("env")  # noqa: F821  — מוזרק על ידי PlatformIO
    _PIO = True
except (NameError, Exception):
    _PIO = False


def _define(env, name):
    """שליפת ערך של -D NAME=\\"value\\" מדגלי הבנייה."""
    for item in env.get("CPPDEFINES", []):
        if isinstance(item, (list, tuple)) and len(item) == 2 and item[0] == name:
            return str(item[1]).strip('\\"')
    return None


def _post_action(source, target, env):
    binary = str(target[0])
    version = _define(env, "PROJECT_VER")
    project = _define(env, "DEVICE_MODEL")

    missing = [n for n, v in (("PROJECT_VER", version), ("DEVICE_MODEL", project)) if not v]
    if missing:
        print(f"*** stamp_app_desc: חסרים דגלים {', '.join(missing)} ב-platformio.ini")
        env.Exit(1)

    try:
        before = stamp(binary, version, project)
        verify(binary, version, project)
    except Exception as e:
        print(f"*** stamp_app_desc נכשל: {e}")
        env.Exit(1)
        return

    print(f"stamp_app_desc: {before[1]} {before[0]}  ->  {project} {version}")


if _PIO:
    env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", _post_action)  # noqa: F821


# ═══════════════════════════════════════════════════════════════
# הרצה ידנית: python stamp_app_desc.py firmware.bin 7.01 JC6416
# ═══════════════════════════════════════════════════════════════

if __name__ == "__main__" and not _PIO:
    if len(sys.argv) != 4:
        print(__doc__)
        print("שימוש: python stamp_app_desc.py <firmware.bin> <גרסה> <דגם>")
        sys.exit(1)
    path, ver, proj = sys.argv[1], sys.argv[2], sys.argv[3]
    try:
        before = stamp(path, ver, proj)
        verify(path, ver, proj)
    except Exception as exc:
        print(f"[שגיאה] {exc}")
        sys.exit(1)
    print(f"{Path(path).name}: {before[1]} {before[0]}  ->  {proj} {ver}")