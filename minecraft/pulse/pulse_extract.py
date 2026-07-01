#!/usr/bin/env python3
import sys
import tarfile
import os

def extract(pulse_path, output_dir):
    os.makedirs(output_dir, exist_ok=True)
    with tarfile.open(pulse_path, "r:gz") as tar:
        tar.extractall(path=output_dir)
    print(f"[OK] Extracted pulse to: {output_dir}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: pulse_extract.py microcraft.pulse out_dir")
        sys.exit(1)
    extract(sys.argv[1], sys.argv[2])
