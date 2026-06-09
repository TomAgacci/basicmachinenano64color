#!/usr/bin/env python3
import os, sys, struct

def write_u32(f, n):
    f.write(struct.pack(">I", n))

def pack_dir(root, out_path):
    entries = []
    for base, _, files in os.walk(root):
        for name in files:
            path = os.path.join(base, name)
            rel = os.path.relpath(path, root)
            with open(path, "rb") as fh:
                data = fh.read()
            entries.append((rel, data))

    with open(out_path, "wb") as f:
        f.write(b"PULSEPKG0001")
        write_u32(f, len(entries))
        for rel, data in entries:
            rb = rel.encode("utf-8")
            write_u32(f, len(rb))
            f.write(rb)
            write_u32(f, len(data))
            f.write(data)

    print(f"Packed {len(entries)} entries into {out_path}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: pulse_pack.py <directory> <out.pulse>")
        sys.exit(1)
    pack_dir(sys.argv[1], sys.argv[2])
