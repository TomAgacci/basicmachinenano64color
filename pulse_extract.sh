#!/usr/bin/env sh
set -e

# Output filenames
PULSE_B64="bundle.pulse.b64"
PULSE_BIN="bundle.pulse"

echo "Writing embedded Base64 pulse bundle..."
cat > "$PULSE_B64" << 'EOF_PULSE'
REPLACE_WITH_YOUR_BASE64_PULSE_DATA
EOF_PULSE

echo "Decoding .pulse bundle..."
python3 - << 'EOF_DECODE'
import base64

with open("bundle.pulse.b64","r") as f:
    data = base64.b64decode(f.read())

with open("bundle.pulse","wb") as f:
    f.write(data)

print("Decoded bundle.pulse ({} bytes)".format(len(data)))
EOF_DECODE

echo "Extracting .pulse contents..."
python3 - << 'EOF_EXTRACT'
import struct, os

def read_u32(f):
    return struct.unpack(">I", f.read(4))[0]

with open("bundle.pulse","rb") as f:
    header = f.read(12)
    if not header.startswith(b"PULSEPKG"):
        raise SystemExit("Invalid .pulse header")

    count = read_u32(f)
    print("Entries:", count)

    for _ in range(count):
        plen = read_u32(f)
        path = f.read(plen).decode()
        size = read_u32(f)
        blob = f.read(size)

        d = os.path.dirname(path)
        if d:
            os.makedirs(d, exist_ok=True)

        with open(path,"wb") as out:
            out.write(blob)

        print("Extracted", path, f"({size} bytes)")
EOF_EXTRACT

echo "Done. Pulse bundle extracted."
