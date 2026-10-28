"""Check source and release packages for removed profile features."""

import argparse
from pathlib import Path
import re
import struct
import subprocess
import sys
import xml.etree.ElementTree as ET


ROOT = Path(__file__).resolve().parents[1]
FORBIDDEN = re.compile(
    r"achievement|achievo|acheivo|gamerscore|\.spa\b|\.xlast\b|spa\.bin|"
    r"UpdatePresence|XboxContext", re.IGNORECASE
)
SOURCE_SUFFIXES = {
    ".c", ".cpp", ".h", ".hpp", ".vcproj", ".vcxproj", ".sln",
    ".xui", ".3xui", ".xml", ".bat", ".ps1", ".xbx",
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def audit_source():
    names = subprocess.check_output(
        ["git", "ls-files", "-z", "--cached", "--others", "--exclude-standard"],
        cwd=ROOT,
    ).decode("utf-8").split("\0")
    count = 0
    for name in sorted(set(names) - {""}):
        path = ROOT / name
        if not path.is_file() or name.startswith(("tools/", ".github/")):
            continue
        require(not FORBIDDEN.search(name), f"Removed metadata file: {name}")
        if path.suffix.lower() not in SOURCE_SUFFIXES:
            continue
        raw = path.read_bytes()
        text = raw.decode("utf-16" if raw[:2] in (b"\xff\xfe", b"\xfe\xff") else "cp1252")
        match = FORBIDDEN.search(text)
        require(match is None, f"Removed feature reference in {name}: {match}")
        if path.suffix.lower() in {".vcproj", ".vcxproj", ".xui", ".3xui", ".xml"}:
            ET.fromstring(text)
        count += 1
    print(f"Source audit passed: {count} source, UI, and project files.")


def audit_xex(path):
    # Header layout: xenia-project/xenia, src/xenia/kernel/util/xex2_info.h.
    # Resource info has optional-header key 0x000002FF. Reject the whole table
    # because this application has no remaining embedded resource sections.
    data = path.read_bytes()
    require(len(data) >= 24 and data[:4] == b"XEX2", "Invalid XEX2 header")
    header_size = struct.unpack_from(">I", data, 8)[0]
    count = struct.unpack_from(">I", data, 20)[0]
    require(24 + count * 8 <= header_size <= len(data), "Invalid XEX header size")
    entries = [struct.unpack_from(">II", data, 24 + i * 8) for i in range(count)]
    require(len({key for key, _ in entries}) == count, "Duplicate XEX optional headers")
    require(not any(key == 0x2FF for key, _ in entries), "XEX still has embedded resource metadata")
    execution = [offset for key, offset in entries if key == 0x40006]
    require(len(execution) == 1, "Missing XEX execution information")
    require(execution[0] + 24 <= header_size, "Invalid XEX execution information")
    title = struct.unpack_from(">I", data, execution[0] + 12)[0]
    require(title == 0xFFED0707, f"Unexpected title ID: {title:08X}")
    print(f"XEX audit passed: title {title:08X}, no embedded resource table.")


def audit_package(package):
    required = (
        "Snes360.xex", "media/Snes360.xzp", "media/Snes360.ttf",
        "media/XuiEffect_Texture.png", "media/saveicon.png", "saveicon.png",
        "media/Shaders/ShadeXuiEffectVertex.xvu", "media/Shaders/ShadeXuiEffectPixel.xpu",
        "README.md", "LICENSE", "snes9x-license.txt",
    )
    for name in required:
        path = package / name
        require(path.is_file() and path.stat().st_size > 0, f"Missing or empty package file: {name}")
    version = (ROOT / "VERSION").read_text().strip()
    require((package / f"release-{version}.md").is_file(), "Missing current release notes")
    for path in package.rglob("*"):
        if path.is_file():
            require(path.suffix.lower() not in {".spa", ".xlast", ".gpd"}, f"Old package metadata: {path}")
    audit_xex(package / "Snes360.xex")
    skin = (package / "media/Snes360.xzp").read_bytes()
    require(len(skin) >= 12 and skin[:4] == b"XUIZ", "Invalid XUI archive header")
    require(struct.unpack_from(">I", skin, 8)[0] == len(skin), "Invalid XUI archive size")
    for scene in ("skin", "Mario", "RomList", "FavoritesListScene", "InGameOptions"):
        name = f"..\\Xbox\\Skin\\{scene}.xur"
        require(name.encode("utf-16-be") in skin, f"Missing runtime scene in XZP: {scene}")
    for word in ("achievement", "achievo", "gamerscore"):
        for encoding in ("ascii", "utf-16-le", "utf-16-be"):
            require(word.encode(encoding) not in skin.lower(), f"Old menu string in XZP: {word}")
    print("Package audit passed: required files present; no old menu strings in XZP.")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--package", type=Path, help="Directory containing Snes360.xex and media")
    parser.add_argument("--map", type=Path, help="Linker map from the same build")
    args = parser.parse_args()
    try:
        audit_source()
        if args.package:
            audit_package(args.package)
        if args.map:
            symbols = args.map.read_text(encoding="cp1252")
            require("Publics by Value" in symbols, "Invalid linker map")
            require(not re.search(r"achievement|achievo|acheivo", symbols, re.IGNORECASE),
                    "Achievement functions remain in the linked executable")
            print("Linker map audit passed: no achievement functions.")
    except (ValueError, OSError, ET.ParseError, subprocess.CalledProcessError, struct.error) as error:
        print(f"Audit failed: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
