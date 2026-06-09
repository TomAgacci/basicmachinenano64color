#!/usr/bin/env sh
set -e

echo "Paste your Base64 .pulse file below. End with CTRL+D."
cat > repo.pulse.b64

python3 - << 'EOF'
import base64
data = base64.b64decode(open("repo.pulse.b64").read())
open("repo.pulse","wb").write(data)
EOF

python3 tools/pulse_extract.py repo.pulse
echo "Extraction complete."
