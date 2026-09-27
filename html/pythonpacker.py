import os, zlib, struct

def pack_pcf(folder, output):
    files = []
    for root, dirs, fs in os.walk(folder):
        for f in fs:
            path = os.path.join(root, f)
            with open(path, "rb") as fp:
                data = fp.read()
            comp = zlib.compress(data)
            files.append((f, comp, data))

    with open(output, "wb") as out:
        out.write(b"PCF1")
        out.write(struct.pack("<I", len(files)))

        for name, comp, raw in files:
            out.write(struct.pack("<B", len(name)))
            out.write(name.encode("ascii"))
            out.write(struct.pack("<I", len(comp)))
            out.write(struct.pack("<I", len(raw)))
            out.write(comp)

    print("Packed", len(files), "files into", output)

pack_pcf("myfolder", "project.pcf")
