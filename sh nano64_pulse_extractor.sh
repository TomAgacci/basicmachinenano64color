#!/usr/bin/env sh
set -e

PULSE_FILE="nano64_repo.pulse"
B64_FILE="nano64_repo.pulse.b64"

cat > "$B64_FILE" << 'EOF_B64'
REPLACE_THIS_WITH_BASE64_OF_NANO64_REPO_PULSE
EOF_B64

python3 - << 'EOF_PULSE'
import base64, struct, os

b64_path = "nano64_repo.pulse.b64"
pulse_path = "nano64_repo.pulse"

with open(b64_path, "r") as f:
    b64 = f.read()
data = base64.b64decode(b64)
with open(pulse_path, "wb") as f:
    f.write(data)
print(f"Wrote {pulse_path} ({len(data)} bytes)")

def read_u32(f):
    import struct
    return struct.unpack(">I", f.read(4))[0]

with open(pulse_path, "rb") as f:
    header = f.read(12)
    if not header.startswith(b"PULSEPKG"):
        raise SystemExit("Invalid .pulse header")
    count = read_u32(f)
    print("Entries:", count)
    for _ in range(count):
        plen = read_u32(f)
        path = f.read(plen).decode("utf-8")
        size = read_u32(f)
        blob = f.read(size)
        d = os.path.dirname(path)
        if d:
            os.makedirs(d, exist_ok=True)
        with open(path, "wb") as out:
            out.write(blob)
        print("Extracted", path, f"({size} bytes)")
EOF_PULSE

echo "Done. Pulse bundle decoded and extracted."
