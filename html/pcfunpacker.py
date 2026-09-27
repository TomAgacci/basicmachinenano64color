import struct, zlib

def unpack_pcf(pcf_file, out_folder):
    with open(pcf_file, "rb") as fp:
        magic = fp.read(4)
        if magic != b"PCF1":
            raise ValueError("Not a PCF file")

        count = struct.unpack("<I", fp.read(4))[0]

        for _ in range(count):
            name_len = struct.unpack("<B", fp.read(1))[0]
            name = fp.read(name_len).decode("ascii")
            comp_len = struct.unpack("<I", fp.read(4))[0]
            raw_len = struct.unpack("<I", fp.read(4))[0]
            comp = fp.read(comp_len)
            raw = zlib.decompress(comp)

            with open(os.path.join(out_folder, name), "wb") as out:
                out.write(raw)

    print("Extracted", count, "files.")

unpack_pcf("project.pcf", "output_folder")
