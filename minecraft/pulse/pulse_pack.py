#!/usr/bin/env python3
import json
import sys
import os
import tarfile

def pack(spec_path, output_path):
    with open(spec_path, "r") as f:
        spec = json.load(f)

    with tarfile.open(output_path, "w:gz") as tar:
        for section in ("firmware", "programs", "assets"):
            for path in spec.get(section, []):
                if not os.path.exists(path):
                    print(f"[WARN] Missing {section} file: {path}")
                    continue
                tar.add(path, arcname=path)
    print(f"[OK] Packed pulse: {output_path}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: pulse_pack.py pulse_spec.json microcraft.pulse")
        sys.exit(1)
    pack(sys.argv[1], sys.argv[2])
