#!/usr/bin/env python3
import struct, sys, os

def read_u32(f):
    return struct.unpack(">I", f.read(4))[0]

def extract_pulse(path):
    with open(path, "rb") as f:
        header = f.read(12)
        if not header.startswith(b"PULSEPKG"):
            raise ValueError("Not a valid .pulse bundle")

        count = read_u32(f)
        print(f"Entries: {count}")

        for _ in range(count):
            plen = read_u32(f)
            file_path = f.read(plen).decode("utf-8")
            size = read_u32(f)
            data = f.read(size)

            d = os.path.dirname(file_path)
            if d:
                os.makedirs(d, exist_ok=True)
            with open(file_path, "wb") as out:
                out.write(data)

            print(f"Extracted {file_path} ({size} bytes)")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: pulse_extract.py <bundle.pulse>")
        sys.exit(1)
    extract_pulse(sys.argv[1])
