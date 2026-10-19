#!/usr/bin/env python3
import sys, os, json

# ------------------------------------------------------------
# BASIC → screen/colors/sprites/VIC parser
# ------------------------------------------------------------

def parse_nano64_basic_to_screen(bas_text):
    screen = [32]*1000
    colors = [1]*1000
    sprites = {}
    fg = 1
    bg = 0
    cur_x = 0
    cur_y = 0
    vic = {"border": 6, "background": 0, "scrollX": 0, "scrollY": 0}

    lines = bas_text.splitlines()

    for line in lines:
        line = line.strip()
        if not line or not line[0].isdigit():
            continue

        try:
            _, cmd = line.split(" ", 1)
        except ValueError:
            continue

        parts = cmd.split()

        # COLOR fg,bg
        if parts[0].upper() == "COLOR":
            args = parts[1].split(",")
            fg = int(args[0])
            bg = int(args[1])
            vic["background"] = bg

        # PLOT x,y,c
        elif parts[0].upper() == "PLOT":
            args = parts[1].split(",")
            x = int(args[0]); y = int(args[1]); c = int(args[2])
            if 0 <= x < 40 and 0 <= y < 25:
                idx = y*40 + x
                screen[idx] = c
                colors[idx] = fg

        # BOX x,y,w,h,c
        elif parts[0].upper() == "BOX":
            args = parts[1].split(",")
            x = int(args[0]); y = int(args[1])
            w = int(args[2]); h = int(args[3])
            c = int(args[4])
            for yy in range(y, y+h):
                for xx in range(x, x+w):
                    if 0 <= xx < 40 and 0 <= yy < 25:
                        idx = yy*40 + xx
                        screen[idx] = c
                        colors[idx] = fg

        # PRINT
        elif parts[0].upper() == "PRINT":
            text = cmd.split("PRINT",1)[1].strip()
            if text.startswith('"') and text.endswith('"'):
                text = text[1:-1]
            for ch in text:
                if ch == '\r':
                    cur_x = 0
                    cur_y += 1
                else:
                    if 0 <= cur_x < 40 and 0 <= cur_y < 25:
                        idx = cur_y*40 + cur_x
                        screen[idx] = ord(ch)
                        colors[idx] = fg
                    cur_x += 1
                    if cur_x >= 40:
                        cur_x = 0
                        cur_y += 1
                if cur_y >= 25:
                    for row in range(1,25):
                        for col in range(40):
                            src = row*40 + col
                            dst = (row-1)*40 + col
                            screen[dst] = screen[src]
                            colors[dst] = colors[src]
                    for col in range(40):
                        idx = 24*40 + col
                        screen[idx] = 32
                        colors[idx] = fg
                    cur_y = 24

        # SPRITE idx,x,y,color
        elif parts[0].upper() == "SPRITE":
            args = parts[1].split(",")
            idx = int(args[0])
            x   = int(args[1])
            y   = int(args[2])
            col = int(args[3])
            sprites[idx] = {
                "x":x, "y":y, "w":8, "h":8,
                "color":col, "color1":2, "color2":3,
                "frame":0, "sheet":"player"
            }

        # SPRITEMOVE idx,dx,dy
        elif parts[0].upper() == "SPRITEMOVE":
            args = parts[1].split(",")
            idx = int(args[0])
            dx  = int(args[1])
            dy  = int(args[2])
            if idx in sprites:
                sprites[idx]["x"] += dx
                sprites[idx]["y"] += dy

        # SPRITEFRAME idx,frame
        elif parts[0].upper() == "SPRITEFRAME":
            args = parts[1].split(",")
            idx = int(args[0])
            frame = int(args[1])
            if idx in sprites:
                sprites[idx]["frame"] = frame

        # SCROLL dx,dy
        elif parts[0].upper() == "SCROLL":
            args = parts[1].split(",")
            vic["scrollX"] += int(args[0])
            vic["scrollY"] += int(args[1])

    return screen, colors, sprites, vic

# ------------------------------------------------------------
# Collision detection
# ------------------------------------------------------------

def check_collisions(sprites):
    hits = []
    keys = list(sprites.keys())
    for i in range(len(keys)):
        for j in range(i+1, len(keys)):
            a = sprites[keys[i]]
            b = sprites[keys[j]]
            if (a["x"] < b["x"] + b["w"] and
                a["x"] + a["w"] > b["x"] and
                a["y"] < b["y"] + b["h"] and
                a["y"] + a["h"] > b["y"]):
                hits.append((keys[i], keys[j]))
    return hits

# ------------------------------------------------------------
# PRG → BASIC (stub)
# ------------------------------------------------------------

def prg_to_basic(prg_bytes):
    return "10 PRINT \"PRG LOADED\"\n"

# ------------------------------------------------------------
# D64 → PRG (stub)
# ------------------------------------------------------------

def d64_extract_first_prg(path):
    return b"\x01\x08\x97\x22\x00"

# ------------------------------------------------------------
# MAIN
# ------------------------------------------------------------

def main():
    if len(sys.argv) < 3:
        print("Usage: program_json_converter.py <input.bas|input.prg|input.d64> <output.json>")
        return

    inp = sys.argv[1]
    out = sys.argv[2]

    if inp.lower().endswith(".bas"):
        bas_text = open(inp).read()
    elif inp.lower().endswith(".prg"):
        prg = open(inp, "rb").read()
        bas_text = prg_to_basic(prg)
    elif inp.lower().endswith(".d64"):
        prg = d64_extract_first_prg(inp)
        bas_text = prg_to_basic(prg)
    else:
        print("Unknown input type:", inp)
        return

    screen, colors, sprites, vic = parse_nano64_basic_to_screen(bas_text)
    collisions = check_collisions(sprites)

    with open(out, "w") as f:
        json.dump({
            "screen": screen,
            "colors": colors,
            "sprites": sprites,
            "vic": vic,
            "collisions": collisions
        }, f, indent=2)

    print("program.json written to", out)

if __name__ == "__main__":
    main()
